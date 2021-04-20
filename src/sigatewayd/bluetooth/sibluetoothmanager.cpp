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

const QBluetoothUuid SICharacteristicUUID {QStringLiteral("f3c2d800-8421-44b1-9655-0951992f313b")}; // NOLINT(cert-err58-cpp)
const QBluetoothUuid RXCharacteristicUUID {QStringLiteral("f3c2d801-8421-44b1-9655-0951992f313b")}; // NOLINT(cert-err58-cpp)
const QBluetoothUuid TXCharacteristicUUID {QStringLiteral("f3c2d802-8421-44b1-9655-0951992f313b")}; // NOLINT(cert-err58-cpp)

SIBluetoothManager::SIBluetoothManager(SIContext* context, QObject* parent):
    QObject(parent), context_(context) {

    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceMessageReceived, this, &SIBluetoothManager::onDeviceMessageReceived_);
}

SIBluetoothManager::~SIBluetoothManager() {
    if (protocol_ != nullptr) {
        context_->deviceAccessManager().unsubscribeFromAllProperties(protocol_);
        delete protocol_;
    }
}

void SIBluetoothManager::startAdvertise() {
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
            if (!frame.validateParameters({{
                {QVariant::String, QVariant::Invalid},
                {QVariant::String, QVariant::Invalid},
                {QVariant::Int, QVariant::Invalid}
            }})) {
                sendFrame_({SIBluetoothProtocolFrame::ERROR, {"invalid frame"}});
                return;
            } else {
                auto accessLevel = SIAccessLevel::None;

                if (SISettings::sharedSettings().authorizeEnabled() && !frame.parameters()[0].isNull() && !frame.parameters()[1].isNull()) {
                    auto user = frame.parameters()[0].toString();
                    auto pass = frame.parameters()[1].toString();

                    if (context_->userAuthorizer() != nullptr) {
                        accessLevel = context_->userAuthorizer()->authorizeUser(user, pass);
                    }
                } else {
                    accessLevel = SISettings::sharedSettings().authorizeGuestAccessLevel();
                }


                if (accessLevel == SIAccessLevel::None) {
                    sendFrame_({SIBluetoothProtocolFrame::ERROR, {"authorization failed"}});
                    return;
                }

                auto version = 1;
                if (!frame.parameters()[2].isNull()) {
                    bool conversionOk = false;
                    version = frame.parameters()[2].toInt(&conversionOk);
                    if (!conversionOk) {
                        sendFrame_({SIBluetoothProtocolFrame::ERROR, {"invalid version"}});
                        return;
                    }
                }

                switch (version) {
                    case 1:
                        protocol_ = new SIBluetoothProtocolV1(accessLevel);
                        connect(protocol_, &SIAbstractBluetoothProtocol::frameReadyToSend, this, &SIBluetoothManager::sendFrame_);
                        sendFrame_({SIBluetoothProtocolFrame::AUTHORIZED, {(unsigned int)accessLevel, version, OPENSTUDER_GATEWAY_VERSION}});
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
        auto response = protocol_->handleFrame(frame, *context_);
        if (response.command() != SIBluetoothProtocolFrame::INVALID) {
            sendFrame_(response);
        }
    }
}

void SIBluetoothManager::onDisconnected_() {
    if (protocol_ != nullptr) {
        context_->deviceAccessManager().unsubscribeFromAllProperties(protocol_);
        delete protocol_;
        protocol_ = nullptr;
    }
    delete peripheral_;
    QMetaObject::invokeMethod(this, &SIBluetoothManager::startAdvertise, Qt::QueuedConnection);
}

void SIBluetoothManager::onDeviceMessageReceived_(const SIDeviceMessage& message) {
    if (protocol_ != nullptr) {
        sendFrame_(protocol_->convertDeviceMessage(message));
    }
}

void SIBluetoothManager::sendFrame_(const SIBluetoothProtocolFrame& frame) {
    service_->writeCharacteristic(service_->characteristic(RXCharacteristicUUID), frame.toBytes());
}
