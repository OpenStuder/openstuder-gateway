#include "sibluetoothmanager.h"
#include "sibluetoothprotocolframe.h"
#include "sibluetoothprotocolv1.h"
#include "../sisettings.h"
#include <siaccesslevel.h>
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
    connect(deviceAccessManager_, &SIDeviceAccessManager::deviceMessageReceived, this, &SIBluetoothManager::onDeviceMessageReceived_);
}

SIBluetoothManager::~SIBluetoothManager() {
    delete protocol_;
}

void SIBluetoothManager::startAdvertise() {
    QLowEnergyAdvertisingData advertisingData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setLocalName(peripheralName_);

    QLowEnergyAdvertisingData scanResponseData;
    advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);
    advertisingData.setLocalName(peripheralName_);
    advertisingData.setServices({SICharacteristicUUID});

    peripheral_->startAdvertising({}, advertisingData, scanResponseData);
}

void SIBluetoothManager::onCharacteristicChanged_(const QLowEnergyCharacteristic& characteristic, const QByteArray& value) {
    Q_UNUSED(characteristic)

    auto frame = SIBluetoothProtocolFrame::fromBytes(value);

    if (protocol_ == nullptr) {
        if (frame.command() == SIBluetoothProtocolFrame::AUTHORIZE) {
            if (!frame.isParameterCountInRange(0, 3)) {
                sendFrame_({SIBluetoothProtocolFrame::ERROR, {"invalid parameter count"}});
                return;
            } else {
                auto accessLevel = SIAccessLevel::Basic;

                if (SISettings::sharedSettings().authorizeEnabled() && frame.parameterCount() >= 2) {
                    auto user = frame.parameters()[0];
                    auto pass = frame.parameters()[1];

                    // TODO: Authorize user!
                } else {
                    accessLevel = SISettings::sharedSettings().authorizeGuestAccessLevel();
                }

                auto versionString = frame.parameterCount() == 3 ? frame.parameters()[2] : "1";

                if (accessLevel == SIAccessLevel::None) {
                    sendFrame_({SIBluetoothProtocolFrame::ERROR, {"authorize failed"}});
                    return;
                }

                bool conversionOk = false;
                int version = versionString.toInt(&conversionOk);
                if (!conversionOk) {
                    sendFrame_({SIBluetoothProtocolFrame::ERROR, {"invalid version"}});
                    return;
                }

                switch (version) {
                    case 1:
                        protocol_ = new SIBluetoothProtocolV1(accessLevel);
                        sendFrame_({SIBluetoothProtocolFrame::AUTHORIZED, {QString::number(version)}});
                        break;

                    default:
                        sendFrame_({SIBluetoothProtocolFrame::ERROR, {"version not supported"}});
                        return;
                }
            }
        } else {
            sendFrame_({SIBluetoothProtocolFrame::ERROR, {"authorization required"}});
        }
    } else {
        auto response = protocol_->handleFrame(frame, deviceAccessManager_);
        if (response.command() != SIBluetoothProtocolFrame::INVALID) {
            sendFrame_(response);
        }
    }
}

void SIBluetoothManager::onDisconnected_() {
    if (protocol_ != nullptr) {
        deviceAccessManager_->unsubscribeFromAllProperties(protocol_);
        delete protocol_;
        protocol_ = nullptr;
    }
    QMetaObject::invokeMethod(this, &SIBluetoothManager::startAdvertise, Qt::QueuedConnection);
}

void SIBluetoothManager::onDeviceMessageReceived_(const QString& deviceAccessID, const SIDeviceMessage& message) {
    if (protocol_ != nullptr) {
        sendFrame_(protocol_->convertDeviceMessage(deviceAccessID, message));
    }
}

void SIBluetoothManager::sendFrame_(const SIBluetoothProtocolFrame& frame) {
    service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), frame.toBytes());
}
