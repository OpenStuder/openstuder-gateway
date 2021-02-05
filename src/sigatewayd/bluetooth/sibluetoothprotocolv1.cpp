#include "sibluetoothprotocolv1.h"

SIBluetoothProtocolV1::SIBluetoothProtocolV1(SIAccessLevel accessLevel): accessLevel_(accessLevel) {}

SIBluetoothProtocolFrame SIBluetoothProtocolV1::handleFrame(SIBluetoothProtocolFrame& frame, SIDeviceAccessManager* deviceAccessManager) {
    switch (frame.command()) {
        case SIBluetoothProtocolFrame::AUTHORIZE:
            return {SIBluetoothProtocolFrame::ERROR, {"invalid state"}};

        case SIBluetoothProtocolFrame::ENUMERATE: {
            if (frame.parameterCount() != 0) return {SIBluetoothProtocolFrame::ERROR, {"invalid parameter count"}};

            auto* operation = deviceAccessManager->enumerateDevices();
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::enumerationOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::READ_PROPERTY: {
            if (frame.parameterCount() != 1) return {SIBluetoothProtocolFrame::ERROR, {"invalid parameter count"}};

            SIGlobalPropertyID id(frame.parameters().first());
            if (!id.isValid()) return {SIBluetoothProtocolFrame::ERROR, {"invalid property id"}};

            auto* operation = deviceAccessManager->readProperty(id);
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::readPropertyOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::WRITE_PROPERTY: {
            if (!frame.isParameterCountInRange(1, 3)) return {SIBluetoothProtocolFrame::ERROR, {"invalid parameter count"}};

            SIGlobalPropertyID id(frame.parameters().first());
            if (!id.isValid()) return {SIBluetoothProtocolFrame::ERROR, {"invalid property id"}};

            QVariant value;
            if (frame.parameterCount() >= 2) {
                value = frame.parameters()[1];
            }

            SIPropertyWriteFlags writeFlags = SIPropertyWriteFlag::Default;
            if (frame.parameterCount() >= 3) {
                // TODO: writeFlags =
            }

            auto* operation = deviceAccessManager->writeProperty(id, value, writeFlags);
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::writePropertyOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::SUBSCRIBE_PROPERTY: {
            if (frame.parameterCount() != 1) return {SIBluetoothProtocolFrame::ERROR, {"invalid parameter count"}};

            SIGlobalPropertyID id(frame.parameters().first());
            if (!id.isValid()) return {SIBluetoothProtocolFrame::ERROR, {"invalid property id"}};

            deviceAccessManager->subscribeToProperty(frame.parameters().first(), this);
            return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {QString::number((int)SIStatus::Success), id.toString()}};   // TODO: Real status.
        }

        default:
            return {SIBluetoothProtocolFrame::ERROR, {"invalid command"}};
    }
}

SIBluetoothProtocolFrame SIBluetoothProtocolV1::convertDeviceMessage(const QString& deviceAccessID, const SIDeviceMessage& message) {
    return {SIBluetoothProtocolFrame::DEVICE_MESSAGE,{deviceAccessID, message.deviceID, QString::number(message.messageID)}};
}


void SIBluetoothProtocolV1::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_UPDATE, {id.toString(), value.toString()}});
}

void SIBluetoothProtocolV1::enumerationOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIDeviceEnumerationOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::ENUMERATED, {QString::number((int)status), QString::number(operation->numberOfDevicesPresent())}});
    delete operation;
}

void SIBluetoothProtocolV1::readPropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyReadOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_READ, {QString::number((int)status), operation->id().toString(), operation->value().toString()}});
    delete operation;
}

void SIBluetoothProtocolV1::writePropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyWriteOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_WRITTEN, {QString::number((int)status)}});
    delete operation;
}
