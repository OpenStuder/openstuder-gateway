#include "xcom485ideviceaccess.h"
#include "xcom485idevice.h"
#include "xcom485ixtender.h"
#include <QCoreApplication>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(XCOM485i, "driver.xcom485i")

static QMap<quint64,QString> xcom485iMessages_ =
#include "xcom485imessages.json"
;

XCom485iDeviceAccess::XCom485iDeviceAccess(const QString& id): SIDeviceAccess(id), deviceOffset_(0) {}

bool XCom485iDeviceAccess::open(const QString& port, unsigned int baudRate, unsigned int deviceOffset) {
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
    while (!reply->isFinished()) QCoreApplication::processEvents();
    if (reply->error() != QModbusDevice::NoError) {
        qCCritical(XCOM485i) << "Error reading pending messages count:" << reply->errorString();
        return;
    }

    auto pendingMessagesCount = reply->result().value(0);

    qCInfo(XCOM485i) << "Reading" << pendingMessagesCount << "pending messages";

    while (pendingMessagesCount > 0) {
        reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, 1, 4}, deviceOffset_ + 1);
        while (!reply->isFinished()) QCoreApplication::processEvents();
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
    // Create list of all existing xtender devices on bus.
    XCom485iXtender* virtualXtender = nullptr;
    QVector<XCom485iXtender*> xtenders;
    XCom485iDevice* virtualVarioTrack = nullptr;
    QVector<XCom485iDevice*> varioTracks;
    XCom485iDevice* virtualVarioString = nullptr;
    QVector<XCom485iDevice*> varioStrings;
    XCom485iDevice* bsp = nullptr;
    for (auto* device: devices) {
        auto* xcomDevice = dynamic_cast<XCom485iDevice*>(device);
        if (xcomDevice) {
            auto modbusAddress = xcomDevice->modbusAddress();
            if (modbusAddress == 10) {
                virtualXtender = dynamic_cast<XCom485iXtender*>(xcomDevice);
            } else if (modbusAddress >= 11 && modbusAddress <= 19) {
                xtenders << dynamic_cast<XCom485iXtender*>(xcomDevice);
            } else if (modbusAddress == 20) {
                virtualVarioTrack = xcomDevice;
            } else if (modbusAddress >= 21 && modbusAddress <= 25) {
                varioTracks << xcomDevice;
            } else if (modbusAddress == 40) {
                virtualVarioString = xcomDevice;
            } else if (modbusAddress >= 41 && modbusAddress <=55) {
                varioStrings << xcomDevice;
            } else if (modbusAddress == 61) {
                bsp = xcomDevice;
            }
        }
    }

    // Sort xtender list by modbus address.
    std::sort(xtenders.begin(), xtenders.end(), [](XCom485iDevice* a, XCom485iDevice* b) {
        return a->modbusAddress() < b->modbusAddress();
    });

    // Remove the last device as long as it does not respond.
    while (!xtenders.isEmpty() && XCom485iXtender::model(xtenders.last()->modbusAddress(), *this) == XCom485iXtender::Invalid) {
        auto missing = xtenders.takeLast();
        devices.removeAll(missing);
    }

    // Try to add new extenders as long as they respond.
    auto nextExpectedXtenderAddress = 11 + xtenders.count();
    while (nextExpectedXtenderAddress <= 19) {
        auto model = XCom485iXtender::model(nextExpectedXtenderAddress, *this);
        if (model == XCom485iXtender::Invalid) {
            break;
        }

        xtenders << new XCom485iXtender(model, nextExpectedXtenderAddress++);
        devices.append(xtenders.last());
    }

    // If there is at least one xtender on the bus, create the virtual multicast device, if none xtenders are present remove virtual device.
    if (xtenders.isEmpty() && virtualXtender != nullptr) {
        devices.removeAll(virtualXtender);
    }
    if (!xtenders.isEmpty() && virtualXtender == nullptr) {
        virtualXtender = new XCom485iXtender(XCom485iXtender::Multicast, 10);
        devices.append(virtualXtender);
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

SIPropertyReadResult XCom485iDeviceAccess::readInputRegister(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) {
    auto reply = modbus_.sendReadRequest({QModbusDataUnit::InputRegisters, static_cast<int>(registerAddress), 2}, deviceAddress);
    while (!reply->isFinished()) {
        QCoreApplication::processEvents();
    }
    if (reply->error() != QModbusDevice::NoError) return {registerAddress, SIStatus::Error, {}};

    union {
        quint16 i[2];
        float f;
    } value = {{0, 0}};
    value.i[0] = reply->result().value(1);
    value.i[1] = reply->result().value(0);
    return {registerAddress, SIStatus::Success, value.f};
}

