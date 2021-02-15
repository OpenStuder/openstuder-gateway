#include "xcom485ideviceaccess.h"
#include "xcom485idevice.h"
#include "xcom485ixtender.h"
#include "xcom485ivariotrack.h"
#include "xcom485ivariostring.h"
#include "xcom485ibatterymanager.h"
#include <array>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <QTime>

using namespace std;

// TODO: Add virtual extender devices for L1, L2 and L3.

Q_DECLARE_LOGGING_CATEGORY(XCOM485i)

static QMap<quint64, QString> xcom485iMessages_ =
#include "xcom485imessages.json"
;

static bool compareXCom485iDeviceModbusAddress(XCom485iDevice* a, XCom485iDevice* b) {
    return a->modbusAddress() < b->modbusAddress();
}

class XCom485iDeviceEnumerator {
  public:
    XCom485iDeviceEnumerator(QVector<SIDevice*>& devices, XCom485iModbusAccess* modbus);

    bool enumerateFast(std::array<int, 5> numberOfDevicesPerCategory);
    bool enumerateSlow();

  private:
    void updateVirtualXtender_();
    void updateVirtualVarioTrack_();
    void updateVirtualVarioString_();

    QVector<SIDevice*>& devices_;
    XCom485iModbusAccess* modbus_;

    XCom485iXtender* virtualXtender = nullptr;
    QVector<XCom485iXtender*> xtenders;
    XCom485iVarioTrack* virtualVarioTrack = nullptr;
    QVector<XCom485iVarioTrack*> varioTracks;
    XCom485iVarioString* virtualVarioString = nullptr;
    QVector<XCom485iVarioString*> varioStrings;
    XCom485iBatteryManager* batteryManager = nullptr;
};

XCom485iDeviceEnumerator::XCom485iDeviceEnumerator(QVector<SIDevice*>& devices, XCom485iModbusAccess* modbus): devices_(devices), modbus_(modbus) {
    for (auto* device: devices) {
        auto* xcomDevice = dynamic_cast<XCom485iDevice*>(device);
        if (xcomDevice) {
            auto modbusAddress = xcomDevice->modbusAddress();
            if (modbusAddress == 10) {
                virtualXtender = dynamic_cast<XCom485iXtender*>(xcomDevice);
            } else if (modbusAddress >= 11 && modbusAddress <= 19) {
                xtenders << dynamic_cast<XCom485iXtender*>(xcomDevice);
            } else if (modbusAddress == 20) {
                virtualVarioTrack = dynamic_cast<XCom485iVarioTrack*>(xcomDevice);
            } else if (modbusAddress >= 21 && modbusAddress <= 25) {
                varioTracks << dynamic_cast<XCom485iVarioTrack*>(xcomDevice);
            } else if (modbusAddress == 40) {
                virtualVarioString = dynamic_cast<XCom485iVarioString*>(xcomDevice);
            } else if (modbusAddress >= 41 && modbusAddress <= 55) {
                varioStrings << dynamic_cast<XCom485iVarioString*>(xcomDevice);
            } else if (modbusAddress == 61) {
                batteryManager = dynamic_cast<XCom485iBatteryManager*>(xcomDevice);
            }
        }
    }

    // Sort all lists by modbus address.
    std::sort(xtenders.begin(), xtenders.end(), compareXCom485iDeviceModbusAddress);
    std::sort(varioTracks.begin(), varioTracks.end(), compareXCom485iDeviceModbusAddress);
    std::sort(varioStrings.begin(), varioStrings.end(), compareXCom485iDeviceModbusAddress);
}

bool XCom485iDeviceEnumerator::enumerateFast(std::array<int, 5> numberOfDevicesPerCategory) {
    // Delta is positive if Xtenders were added, negative if devices were removed.
    auto delta = numberOfDevicesPerCategory[0] - xtenders.count();

    // If there are new devices, add them.
    if (delta > 0) {
        for (int i = 0; i < delta; ++i) {
            auto xtenderAddress = 11 + xtenders.count();
            auto model = XCom485iXtender::model(xtenderAddress, *modbus_);
            switch (model) {
                case XCom485iXtender::Invalid:
                case XCom485iXtender::Multicast:
                    // TODO: This is a serious error!
                    break;

                case XCom485iXtender::XTH:
                case XCom485iXtender::XTM:
                case XCom485iXtender::XTS:
                    xtenders << new XCom485iXtender(model, xtenderAddress, modbus_);
                    devices_.append(xtenders.last());
                    break;
            }
        }
    } else {
        // If Xtender devices are gone, remove them.
        while (delta < 0) {
            auto removed = xtenders.takeLast();
            devices_.removeOne(removed);
            ++delta;
        }
    }

    updateVirtualXtender_();

    // Delta is positive if VarioTracks were added, negative if devices were removed.
    delta = numberOfDevicesPerCategory[1] - varioTracks.count();

    // If there are new devices, add them.
    if (delta > 0) {
        for (int i = 0; i < delta; ++i) {
            auto varioTrackAddress = 21 + varioTracks.count();
            auto model = XCom485iVarioTrack::model(varioTrackAddress, *modbus_);
            switch (model) {
                case XCom485iVarioTrack::Invalid:
                case XCom485iVarioTrack::Multicast:
                    // TODO: This is a serious error!
                    break;

                case XCom485iVarioTrack::VT65:
                case XCom485iVarioTrack::VT80:
                    varioTracks << new XCom485iVarioTrack(model, varioTrackAddress, modbus_);
                    devices_.append(varioTracks.last());
                    break;
            }
        }
    } else {
        // If VarioTrack devices are gone, remove them.
        while (delta < 0) {
            auto removed = varioTracks.takeLast();
            devices_.removeOne(removed);
            ++delta;
        }
    }

    updateVirtualVarioTrack_();

    // Delta is positive if VarioStrings were added, negative if devices were removed.
    delta = numberOfDevicesPerCategory[2] - varioStrings.count();

    // If there are new devices, add them.
    if (delta > 0) {
        for (int i = 0; i < delta; ++i) {
            auto varioStringAddress = 41 + varioStrings.count();
            auto model = XCom485iVarioString::model(varioStringAddress, *modbus_);
            switch (model) {
                case XCom485iVarioString::Invalid:
                case XCom485iVarioString::Multicast:
                    // TODO: This is a serious error!
                    break;

                case XCom485iVarioString::VS70:
                case XCom485iVarioString::VS120:
                    varioStrings << new XCom485iVarioString(model, varioStringAddress, modbus_);
                    devices_.append(varioStrings.last());
                    break;
            }
        }
    } else {
        // If VarioString devices are gone, remove them.
        while (delta < 0) {
            auto removed = varioStrings.takeLast();
            devices_.removeOne(removed);
            ++delta;
        }
    }

    updateVirtualVarioTrack_();

    switch (numberOfDevicesPerCategory[3]) {
        case 0:
            if (batteryManager != nullptr) {
                devices_.removeOne(batteryManager);
            }
            break;

        case 1: {
            auto model = XCom485iBatteryManager::model(*modbus_);
            if (batteryManager != nullptr && (model == XCom485iBatteryManager::Invalid || batteryManager->xComModel() != model)) {
                devices_.removeOne(batteryManager);
                batteryManager = nullptr;
            }
            if (model != XCom485iBatteryManager::Invalid && batteryManager == nullptr) {
                batteryManager = new XCom485iBatteryManager(model, modbus_);
                devices_.append(batteryManager);
            }
            break;
        }

        default:
            // TODO: This is an error!
            break;
    }

    return true;
}

bool XCom485iDeviceEnumerator::enumerateSlow() {

    // Remove the last device as long as it does not respond.
    while (!xtenders.isEmpty() && XCom485iXtender::model(xtenders.last()->modbusAddress(), *modbus_) == XCom485iXtender::Invalid) {
        auto missing = xtenders.takeLast();
        devices_.removeAll(missing);
    }

    // Try to add new extenders as long as they respond.
    auto nextExpectedXtenderAddress = 11 + xtenders.count();
    while (nextExpectedXtenderAddress <= 19) {
        auto model = XCom485iXtender::model(nextExpectedXtenderAddress, *modbus_);
        if (model == XCom485iXtender::Invalid) {
            break;
        }

        xtenders << new XCom485iXtender(model, nextExpectedXtenderAddress++, modbus_);
        devices_.append(xtenders.last());
    }

    updateVirtualXtender_();

    // Remove the last device as long as it does not respond.
    while (!varioTracks.isEmpty() && XCom485iVarioTrack::model(varioTracks.last()->modbusAddress(), *modbus_) == XCom485iVarioTrack::Invalid) {
        auto missing = varioTracks.takeLast();
        devices_.removeAll(missing);
    }

    // Try to add new VarioTracks as long as they respond.
    auto nextExpectedVarioTrackAddress = 21 + varioTracks.count();
    while (nextExpectedVarioTrackAddress <= 25) {
        auto model = XCom485iVarioTrack::model(nextExpectedVarioTrackAddress, *modbus_);
        if (model == XCom485iVarioTrack::Invalid) {
            break;
        }

        varioTracks << new XCom485iVarioTrack(model, nextExpectedVarioTrackAddress++, modbus_);
        devices_.append(varioTracks.last());
    }

    updateVirtualVarioTrack_();

    // Remove the last device as long as it does not respond.
    while (!varioStrings.isEmpty() && XCom485iVarioString::model(varioStrings.last()->modbusAddress(), *modbus_) == XCom485iVarioString::Invalid) {
        auto missing = varioStrings.takeLast();
        devices_.removeAll(missing);
    }

    // Try to add new VarioStrings as long as they respond.
    auto nextExpectedVarioStringAddress = 41 + varioStrings.count();
    while (nextExpectedVarioStringAddress <= 55) {
        auto model = XCom485iVarioString::model(nextExpectedVarioStringAddress, *modbus_);
        if (model == XCom485iVarioString::Invalid) {
            break;
        }

        varioStrings << new XCom485iVarioString(model, nextExpectedVarioStringAddress++, modbus_);
        devices_.append(varioStrings.last());
    }

    updateVirtualVarioString_();

    // Check for BSP or XCom-CAN BMS device on bus.
    auto batteryManagerModel = XCom485iBatteryManager::model(*modbus_);

    if (batteryManagerModel == XCom485iBatteryManager::Invalid) {
        if (batteryManager != nullptr) {
            // If there is no device on the bus, remove the existing object from devices.
            devices_.removeAll(batteryManager);
        }
    } else {
        // Remove existing battery manager if it is present and not the correct model.
        if (batteryManager != nullptr && batteryManager->xComModel() != batteryManagerModel) {
            devices_.removeAll(batteryManager);
            batteryManager = nullptr;
        }

        // Add battery manager if not already present.
        if (batteryManager == nullptr) {
            batteryManager = new XCom485iBatteryManager(batteryManagerModel, modbus_);
            devices_.append(batteryManager);
        }
    }

    return true;
}

void XCom485iDeviceEnumerator::updateVirtualXtender_() {
    // If there is at least one xtender on the bus, create the virtual multicast device, if none xtenders are present remove virtual device.
    if (xtenders.isEmpty() && virtualXtender != nullptr) {
        devices_.removeAll(virtualXtender);
    }
    if (!xtenders.isEmpty() && virtualXtender == nullptr) {
        virtualXtender = new XCom485iXtender(XCom485iXtender::Multicast, 10, modbus_);
        devices_.append(virtualXtender);
    }
}

void XCom485iDeviceEnumerator::updateVirtualVarioTrack_() {
    // If there is at least one VarioTrack on the bus, create the virtual multicast device, if none VarioTracks are present remove virtual device.
    if (varioTracks.isEmpty() && virtualVarioTrack != nullptr) {
        devices_.removeAll(virtualVarioTrack);
    }
    if (!varioTracks.isEmpty() && virtualVarioTrack == nullptr) {
        virtualVarioTrack = new XCom485iVarioTrack(XCom485iVarioTrack::Multicast, 20, modbus_);
        devices_.append(virtualVarioTrack);
    }
}

void XCom485iDeviceEnumerator::updateVirtualVarioString_() {
    // If there is at least one VarioString on the bus, create the virtual multicast device, if none VarioStrings are present remove virtual device.
    if (varioStrings.isEmpty() && virtualVarioString != nullptr) {
        devices_.removeAll(virtualVarioString);
    }
    if (!varioStrings.isEmpty() && virtualVarioString == nullptr) {
        virtualVarioString = new XCom485iVarioString(XCom485iVarioString::Multicast, 40, modbus_);
        devices_.append(virtualVarioString);
    }
}


XCom485iDeviceAccess::XCom485iDeviceAccess(const QString& id): SIDeviceAccess(id), deviceOffset_(0) {}

bool XCom485iDeviceAccess::open(const QString& port, unsigned int baudRate, quint8 deviceOffset) {
    qCInfo(XCOM485i,) << "Opening modbus on port" << port << ", baudrate =" << baudRate << ", device offset =" << deviceOffset << ", timeout = 1000";

    deviceOffset_ = deviceOffset;

    modbus_.setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
    modbus_.setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudRate);
    modbus_.setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::EvenParity);
    modbus_.setTimeout(1000);
    if (!modbus_.connectDevice()) {
        qCCritical(XCOM485i,) << "Failed to open " << port << ":" << modbus_.errorString();
        return false;
    }

    return true;
}

void XCom485iDeviceAccess::retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const {
    auto reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, 0, 1}, deviceOffset_ + 1);
    while (!reply->isFinished()) { QCoreApplication::processEvents(); }
    if (reply->error() != QModbusDevice::NoError) {
        qCCritical(XCOM485i,) << "Error reading pending messages count:" << reply->errorString();
        return;
    }

    auto pendingMessagesCount = reply->result().value(0);

    qCInfo(XCOM485i,) << "Reading" << pendingMessagesCount << "pending messages";

    while (pendingMessagesCount > 0) {
        reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, 1, 4}, deviceOffset_ + 1);
        while (!reply->isFinished()) { QCoreApplication::processEvents(); }
        if (reply->error() != QModbusDevice::NoError) {
            qCCritical(XCOM485i,) << "Error reading pending message:" << reply->errorString();
            return;
        }
        SIDeviceMessage message;
        message.accessID = id();
        message.deviceID = QString::number(reply->result().value(0));
        message.messageID = reply->result().value(1);
        message.message = xcom485iMessages_.value(message.messageID, "Unknown Message");
        messages.append(std::move(message));
        --pendingMessagesCount;
    }
}

bool XCom485iDeviceAccess::enumerateDevices_(QVector<SIDevice*>& devices) {

    // Create list of all currently existing devices.
    XCom485iDeviceEnumerator enumerator(devices, this);

    // If fast enumeration is disabled, enumerate directly using the slow mechanism.
    if (forceSlowEnumeration_) {
        return enumerator.enumerateSlow();
    } else {
        // Try to get the number of devices present on bus from XCom485i.
        auto reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, 5, 5}, deviceOffset_ + 1);
        while (!reply->isFinished()) {
            QCoreApplication::processEvents();
        }

        if (reply->error() == QModbusDevice::NoError) {

            // If we could get the device count, enumerate the devices using this information, which is way faster.
            return enumerator.enumerateFast({
                                                reply->result().value(0),
                                                reply->result().value(1),
                                                reply->result().value(2),
                                                reply->result().value(3),
                                                reply->result().value(4)
                                            });

        } else {

            // Otherwise fall back to the legacy method which is way slower.
            return enumerator.enumerateSlow();
        }
    }
}

void XCom485iDeviceAccess::completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const {
    Q_UNUSED(flags)

    QJsonObject parameters;
    parameters["port"] = modbus_.connectionParameter(QModbusRtuSerialMaster::SerialPortNameParameter).toString();
    parameters["baudRate"] = modbus_.connectionParameter(QModbusRtuSerialMaster::SerialBaudRateParameter).toInt();
    parameters["deviceOffset"] = static_cast<int>(deviceOffset_);
    object["parameters"] = parameters;
}

SIPropertyReadResult XCom485iDeviceAccess::readInputRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) {
    deviceAddress += deviceOffset_;

    auto reply = unique_ptr<QModbusReply>(modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, static_cast<int>(registerAddress), 2}, deviceAddress));
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) {
        return {registerAddress, SIStatus::Error, {}};
    }

    union {
        quint16 i[2];
        float f;
    } conv = {{0, 0}};
    conv.i[0] = reply->result().value(1);
    conv.i[1] = reply->result().value(0);

    switch (type) {
        case SIPropertyType::Float:
        case SIPropertyType::Enum:
        case SIPropertyType::Bool:
        case SIPropertyType::DaysOfWeek:
            return {registerAddress, SIStatus::Success, conv.f};

        case SIPropertyType::TimeOfDay:
            return {registerAddress, SIStatus::Success, QTime((int)conv.f / 60, (int)conv.f % 60)};

        case SIPropertyType::Invalid:
        case SIPropertyType::Signal:
        default:
            return {registerAddress, SIStatus::Error, {}};
    }
}

SIPropertyReadResult XCom485iDeviceAccess::readHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) {
    deviceAddress += deviceOffset_;

    auto reply = unique_ptr<QModbusReply>(modbus_.sendReadRequest({QModbusDataUnit::HoldingRegisters, static_cast<int>(registerAddress), 2}, deviceAddress));
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) {
        return {registerAddress, SIStatus::Error, {}};
    }

    union {
        quint16 i[2];
        float f;
    } conv = {{0, 0}};
    conv.i[0] = reply->result().value(1);
    conv.i[1] = reply->result().value(0);

    switch (type) {
        case SIPropertyType::Float:
        case SIPropertyType::Enum:
        case SIPropertyType::Bool:
        case SIPropertyType::DaysOfWeek:
            return {registerAddress, SIStatus::Success, conv.f};

        case SIPropertyType::TimeOfDay:
            return {registerAddress, SIStatus::Success, QTime((int)conv.f / 60, (int)conv.f % 60)};

        case SIPropertyType::Invalid:
        case SIPropertyType::Signal:
        default:
            return {registerAddress, SIStatus::Error, {}};
    }
}

SIPropertyWriteResult XCom485iDeviceAccess::writeHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, const QVariant& value, SIPropertyType type) {
    deviceAddress += deviceOffset_;

    union {
        quint16 i[2];
        float f;
    } conv = {{0, 0}};

    switch (type) {
        case SIPropertyType::Float:
        case SIPropertyType::Enum:
        case SIPropertyType::Bool:
        case SIPropertyType::DaysOfWeek:
            conv.f = value.toFloat();
            break;

        case SIPropertyType::TimeOfDay: {
            if (!value.canConvert<QTime>()) {
                return {registerAddress, SIStatus::Error};
            }
            auto time = value.toTime();
            conv.f = (float)time.hour() * 60.0f + (float)time.second();
            break;
        }

        case SIPropertyType::Signal:
            conv.f = 1.0;
            break;

        case SIPropertyType::Invalid:
        default:
            return {registerAddress, SIStatus::Error};
    }

    auto reply = unique_ptr<QModbusReply>(modbus_.sendWriteRequest({QModbusDataUnit::HoldingRegisters, static_cast<int>(registerAddress), {conv.i[1], conv.i[0]}}, deviceAddress));
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) {
        return {registerAddress, SIStatus::Error};
    }

    return {registerAddress, SIStatus::Success};
}
