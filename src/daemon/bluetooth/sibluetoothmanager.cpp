#include "sibluetoothmanager.h"
#include "sibluetoothprotocolframe.h"
#include <QLowEnergyController>
#include <QLowEnergyCharacteristicData>
#include <QLowEnergyServiceData>
#include <QLowEnergyDescriptorData>
#include <QLowEnergyAdvertisingParameters>
#include <QLowEnergyAdvertisingData>


const QBluetoothUuid SICharacteristicUUID {QStringLiteral("12345678-1234-0000-1623-123456789ABC")};
const QBluetoothUuid RXCharacteristicUUID {QStringLiteral("12345678-1234-0001-1623-123456789ABC")};
const QBluetoothUuid TXCharacteristicUUID {QStringLiteral("12345678-1234-0002-1623-123456789ABC")};

SIBluetoothManager::SIBluetoothManager(SIDeviceAccessManager* deviceAccessManager, QObject* parent): QObject(parent), deviceAccessManager_(deviceAccessManager) {
    QLowEnergyCharacteristicData rxCharacteristic;
    rxCharacteristic.setUuid(RXCharacteristicUUID);
    rxCharacteristic.setProperties(QLowEnergyCharacteristic::Notify);
    rxCharacteristic.addDescriptor({QBluetoothUuid::ClientCharacteristicConfiguration, QByteArray(2, 0)});

    QLowEnergyCharacteristicData txCharacteristic;
    txCharacteristic.setUuid(TXCharacteristicUUID);
    txCharacteristic.setProperties(QLowEnergyCharacteristic::WriteNoResponse);

    QLowEnergyServiceData siService;
    siService.setType(QLowEnergyServiceData::ServiceTypePrimary);
    siService.setUuid(SICharacteristicUUID);
    siService.addCharacteristic(rxCharacteristic);
    siService.addCharacteristic(txCharacteristic);

    peripheral_ = QLowEnergyController::createPeripheral(this);
    service_ = peripheral_->addService(siService);
    connect(service_, &QLowEnergyService::characteristicChanged, this, &SIBluetoothManager::onCharacteristicChanged_);
    connect(peripheral_, &QLowEnergyController::disconnected, this, &SIBluetoothManager::onDisconnected_);
}

void SIBluetoothManager::startAdvertise() {
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setLocalName(name_);

    QLowEnergyAdvertisingData scanResponseData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setLocalName(name_);
    advertisingData.setServices({SICharacteristicUUID});

    peripheral_->startAdvertising({}, advertisingData, scanResponseData);
}

void SIBluetoothManager::onCharacteristicChanged_(const QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    auto frame = SIBluetoothProtocolFrame::fromBytes(value);
    auto parameterCount = frame.parameters().count();

    switch (frame.command()) {
        case SIBluetoothProtocolFrame::ENUMERATE:
            if (parameterCount == 0) {
                auto* operation = deviceAccessManager_->enumerateDevices();
                connect(operation, &SIAbstractOperation::finished, [this, operation](SIStatus status) {
                    service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), SIBluetoothProtocolFrame(
                        SIBluetoothProtocolFrame::ENUMERATED,
                        {QString::number((int)status)}
                    ).toBytes());
                    delete operation;
                });
                return;
            }
            break;

        case SIBluetoothProtocolFrame::DESCRIBE:
            // TODO...
            break;

        case SIBluetoothProtocolFrame::READ_PROPERTY:
            if (parameterCount == 1) {
                SIGlobalPropertyID id(frame.parameters().first());
                if (id.isValid()) {
                    auto* operation = deviceAccessManager_->readProperty(id);
                    connect(operation, &SIAbstractOperation::finished, [this, operation, id](SIStatus status) {
                        service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), SIBluetoothProtocolFrame(
                            SIBluetoothProtocolFrame::PROPERTY_READ,
                            {QString::number((int)status), id.toString(), operation->value().toString()}
                        ).toBytes());
                        delete operation;
                    });
                    return;
                }
            }
            break;

        case SIBluetoothProtocolFrame::WRITE_PROPERTY:
            if (parameterCount >= 1 && parameterCount <= 3) {
                SIGlobalPropertyID id(frame.parameters().first());
                QVariant value;
                if (parameterCount >= 2) {
                    value = frame.parameters()[1];
                }
                SIPropertyWriteFlags writeFlags = SIPropertyWriteFlag::Default;
                if (parameterCount >= 3) {
                    // TODO: writeFlags =
                }
                if (id.isValid() && value.isValid()) {
                    auto* operation = deviceAccessManager_->writeProperty(id, value, writeFlags);
                    connect(operation, &SIAbstractOperation::finished, [this, operation, id](SIStatus status) {
                        service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), SIBluetoothProtocolFrame(
                            SIBluetoothProtocolFrame::PROPERTY_WRITTEN,
                            {QString::number((int)status), id.toString()}
                        ).toBytes());
                        delete operation;
                    });
                    return;
                }
            }
            break;

        case SIBluetoothProtocolFrame::SUBSCRIBE_PROPERTY:
            if (frame.parameters().count() == 1) {
                SIGlobalPropertyID id(frame.parameters().first());
                if (id.isValid()) {
                    deviceAccessManager_->subscribeToProperty(frame.parameters().first(), this);
                    service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), SIBluetoothProtocolFrame(
                        SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED,
                        {QString::number((int)SIStatus::Success), id.toString()} // TODO: Real status.
                    ).toBytes());
                    return;
                }
            }
            break;
    }

    service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), SIBluetoothProtocolFrame(SIBluetoothProtocolFrame::ERROR).toBytes());
}

void SIBluetoothManager::onDisconnected_() {
    deviceAccessManager_->unsubscribeFromAllProperties(this);
    QMetaObject::invokeMethod(this, &SIBluetoothManager::startAdvertise, Qt::QueuedConnection);
}

void SIBluetoothManager::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), SIBluetoothProtocolFrame(
        SIBluetoothProtocolFrame::PROPERTY_UPDATE,
        {id.toString(), value.toString()}).toBytes());
}
