#include "xcom485ideviceaccess.h"
#include "xcom485idevice.h"
#include "xcom485ixtender.h"
#include "xcom485ivariotrack.h"
#include "xcom485ivariostring.h"
#include "xcom485ibatterymanager.h"
#include <QCoreApplication>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(XCOM485i)

static QMap<quint64, QString> xcom485iMessages_ =
#include "xcom485imessages.json"
;

XCom485iDeviceAccess::XCom485iDeviceAccess(const QString& id): SIDeviceAccess(id), deviceOffset_(0) {}

bool XCom485iDeviceAccess::open(const QString& port, unsigned int baudRate, quint8 deviceOffset) {
    qCInfo(XCOM485i) << "Opening modbus on port" << port << ", baudrate =" << baudRate << ", device offset =" << deviceOffset << ", timeout = 1000";

    deviceOffset_ = deviceOffset;

    modbus_.setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
    modbus_.setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudRate);
    modbus_.setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::EvenParity);
    modbus_.setTimeout(1000);
    if (!modbus_.connectDevice()) {
        qCCritical(XCOM485i) << "Failed to open " << port << ":" << modbus_.errorString();
        return false;
    }

    return true;
}

void XCom485iDeviceAccess::retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const {
    auto reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, 0, 1}, deviceOffset_ + 1);
    while (!reply->isFinished()) { QCoreApplication::processEvents(); }
    if (reply->error() != QModbusDevice::NoError) {
        qCCritical(XCOM485i) << "Error reading pending messages count:" << reply->errorString();
        return;
    }

    auto pendingMessagesCount = reply->result().value(0);

    qCInfo(XCOM485i) << "Reading" << pendingMessagesCount << "pending messages";

    while (pendingMessagesCount > 0) {
        reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, 1, 4}, deviceOffset_ + 1);
        while (!reply->isFinished()) { QCoreApplication::processEvents(); }
        if (reply->error() != QModbusDevice::NoError) {
            qCCritical(XCOM485i) << "Error reading pending message:" << reply->errorString();
            return;
        }
        SIDeviceMessage message;
        message.deviceID = QString::number(reply->result().value(0));
        message.messageID = reply->result().value(1);
        message.message = xcom485iMessages_.value(message.messageID, "Unknown Message");
        messages.append(std::move(message));
        --pendingMessagesCount;
    }
}

bool XCom485iDeviceAccess::enumerateDevices_(QVector<SIDevice*>& devices) {
    // Create list of all existing devices on bus.
    XCom485iXtender* virtualXtender = nullptr;
    QVector<XCom485iXtender*> xtenders;
    XCom485iVarioTrack* virtualVarioTrack = nullptr;
    QVector<XCom485iVarioTrack*> varioTracks;
    XCom485iVarioString* virtualVarioString = nullptr;
    QVector<XCom485iVarioString*> varioStrings;
    XCom485iBatteryManager* batteryManager = nullptr;
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

    // Sort Xtender list by modbus address.
    std::sort(xtenders.begin(), xtenders.end(), [](XCom485iDevice* a, XCom485iDevice* b) {
        return a->modbusAddress() < b->modbusAddress();
    });

    // Remove the last device as long as it does not respond.
    while (!xtenders.isEmpty() && XCom485iXtender::model(xtenders.last()->modbusAddress(), *this) == XCom485iXtender::Invalid) {
        auto missing = xtenders.takeLast();
        devices.removeAll(missing);
        delete missing;
    }

    // Try to add new extenders as long as they respond.
    auto nextExpectedXtenderAddress = 11 + xtenders.count();
    while (nextExpectedXtenderAddress <= 19) {
        auto model = XCom485iXtender::model(nextExpectedXtenderAddress, *this);
        if (model == XCom485iXtender::Invalid) {
            break;
        }

        xtenders << new XCom485iXtender(model, nextExpectedXtenderAddress++, this);
        devices.append(xtenders.last());
    }

    // If there is at least one xtender on the bus, create the virtual multicast device, if none xtenders are present remove virtual device.
    if (xtenders.isEmpty() && virtualXtender != nullptr) {
        devices.removeAll(virtualXtender);
        delete virtualXtender;
    }
    if (!xtenders.isEmpty() && virtualXtender == nullptr) {
        virtualXtender = new XCom485iXtender(XCom485iXtender::Multicast, 10, this);
        devices.append(virtualXtender);
    }

    // Sort VarioTrack list by modbus address.
    std::sort(varioTracks.begin(), varioTracks.end(), [](XCom485iDevice* a, XCom485iDevice* b) {
        return a->modbusAddress() < b->modbusAddress();
    });

    // Remove the last device as long as it does not respond.
    while (!varioTracks.isEmpty() && XCom485iVarioTrack::model(varioTracks.last()->modbusAddress(), *this) == XCom485iVarioTrack::Invalid) {
        auto missing = varioTracks.takeLast();
        devices.removeAll(missing);
        delete missing;
    }

    // Try to add new VarioTracks as long as they respond.
    auto nextExpectedVarioTrackAddress = 21 + varioTracks.count();
    while (nextExpectedVarioTrackAddress <= 25) {
        auto model = XCom485iVarioTrack::model(nextExpectedVarioTrackAddress, *this);
        if (model == XCom485iVarioTrack::Invalid) {
            break;
        }

        varioTracks << new XCom485iVarioTrack(model, nextExpectedVarioTrackAddress++, this);
        devices.append(varioTracks.last());
    }

    // If there is at least one VarioTrack on the bus, create the virtual multicast device, if none VarioTracks are present remove virtual device.
    if (varioTracks.isEmpty() && virtualVarioTrack != nullptr) {
        devices.removeAll(virtualVarioTrack);
        delete virtualVarioTrack;
    }
    if (!varioTracks.isEmpty() && virtualVarioTrack == nullptr) {
        virtualVarioTrack = new XCom485iVarioTrack(XCom485iVarioTrack::Multicast, 20, this);
        devices.append(virtualVarioTrack);
    }

    // Sort VarioString list by modbus address.
    std::sort(varioStrings.begin(), varioStrings.end(), [](XCom485iDevice* a, XCom485iDevice* b) {
        return a->modbusAddress() < b->modbusAddress();
    });

    // Remove the last device as long as it does not respond.
    while (!varioStrings.isEmpty() && XCom485iVarioString::model(varioStrings.last()->modbusAddress(), *this) == XCom485iVarioString::Invalid) {
        auto missing = varioStrings.takeLast();
        devices.removeAll(missing);
        delete missing;
    }

    // Try to add new VarioStrings as long as they respond.
    auto nextExpectedVarioStringAddress = 41 + varioStrings.count();
    while (nextExpectedVarioStringAddress <= 55) {
        auto model = XCom485iVarioString::model(nextExpectedVarioStringAddress, *this);
        if (model == XCom485iVarioString::Invalid) {
            break;
        }

        varioStrings << new XCom485iVarioString(model, nextExpectedVarioStringAddress++, this);
        devices.append(varioStrings.last());
    }

    // If there is at least one VarioString on the bus, create the virtual multicast device, if none VarioStrings are present remove virtual device.
    if (varioStrings.isEmpty() && virtualVarioString != nullptr) {
        devices.removeAll(virtualVarioString);
        delete virtualVarioString;
    }
    if (!varioStrings.isEmpty() && virtualVarioString == nullptr) {
        virtualVarioString = new XCom485iVarioString(XCom485iVarioString::Multicast, 40, this);
        devices.append(virtualVarioString);
    }

    // Check for BSP or XCom-CAN BMS device on bus.
    auto batteryManagerModel = XCom485iBatteryManager::model(*this);

    if (batteryManagerModel == XCom485iBatteryManager::Invalid) {
        if (batteryManager != nullptr) {
            // If there is no device on the bus, remove the existing object from devices.
            devices.removeAll(batteryManager);
            delete batteryManager;
        }
    } else {
        // Remove existing battery manager if it is present and not the correct model.
        if (batteryManager != nullptr && batteryManager->xComModel() != batteryManagerModel) {
            devices.removeAll(batteryManager);
            delete batteryManager;
            batteryManager = nullptr;
        }

        // Add battery manager if not already present.
        if (batteryManager == nullptr) {
            batteryManager = new XCom485iBatteryManager(batteryManagerModel, this);
            devices.append(batteryManager);
        }
    }

    return true;
}

void XCom485iDeviceAccess::completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const {
    QJsonObject parameters;
    parameters["port"] = modbus_.connectionParameter(QModbusRtuSerialMaster::SerialPortNameParameter).toString();
    parameters["baudRate"] = modbus_.connectionParameter(QModbusRtuSerialMaster::SerialBaudRateParameter).toInt();
    parameters["deviceOffset"] = static_cast<int>(deviceOffset_);
    object["parameters"] = parameters;
}

SIPropertyReadResult XCom485iDeviceAccess::readInputRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) {
    deviceAddress += deviceOffset_;

    auto reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, static_cast<int>(registerAddress), 2}, deviceAddress);
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) { return {registerAddress, SIStatus::Error, {}}; }

    union {
        quint16 i[2];
        float f;
    } conv = {{0, 0}};
    conv.i[0] = reply->result().value(1);
    conv.i[1] = reply->result().value(0);

    return {registerAddress, SIStatus::Success, conv.f};
}


SIPropertyReadResult XCom485iDeviceAccess::readHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) {
    deviceAddress += deviceOffset_;

    auto reply = modbus_.sendReadRequest({QModbusDataUnit::HoldingRegisters, static_cast<int>(registerAddress), 2}, deviceAddress);
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) { return {registerAddress, SIStatus::Error, {}}; }

    union {
        quint16 i[2];
        float f;
    } conv = {{0, 0}};
    conv.i[0] = reply->result().value(1);
    conv.i[1] = reply->result().value(0);

    return {registerAddress, SIStatus::Success, conv.f};
}

SIPropertyWriteResult XCom485iDeviceAccess::writeHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, const QVariant& value, SIPropertyType type) {
    deviceAddress += deviceOffset_;

    union {
        quint16 i[2];
        float f;
    } conv = {{0, 0}};

    conv.f = value.toFloat();

    auto reply = modbus_.sendWriteRequest({QModbusDataUnit::HoldingRegisters, static_cast<int>(registerAddress), {conv.i[1], conv.i[0]}}, deviceAddress);
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) { return {registerAddress, SIStatus::Error}; }

    return {registerAddress, SIStatus::Success};
}
