#include "sibluetoothprotocolv1.h"

SIBluetoothProtocolV1::SIBluetoothProtocolV1(SIAccessLevel accessLevel): accessLevel_(accessLevel) {}

SIBluetoothProtocolFrame SIBluetoothProtocolV1::handleFrame(SIBluetoothProtocolFrame& frame, SIContext& context) {
    switch (frame.command()) {
        case SIBluetoothProtocolFrame::AUTHORIZE:
            return {SIBluetoothProtocolFrame::ERROR, {"invalid state"}};

        case SIBluetoothProtocolFrame::ENUMERATE: {
            if (frame.parameterCount() != 0) { return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}}; }

            auto* operation = context.deviceAccessManager().enumerateDevices();
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::enumerationOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::READ_PROPERTY: {
            if (!frame.validateParameters({{QVariant::String}})) {
                return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}};
            }

            SIGlobalPropertyID id(frame.parameters()[0].value<QString>());
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIBluetoothProtocolFrame::PROPERTY_READ, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }
            if (!property.isFlagSet(SIPropertyFlag::Readable)) {
                return {SIBluetoothProtocolFrame::PROPERTY_READ, {(int)SIStatus::Error, frame.parameters()[0]}};
            }

            auto* operation = context.deviceAccessManager().readProperty(id);
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::readPropertyOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::WRITE_PROPERTY: {
            if (!frame.validateParameters({
                                              {QVariant::String},
                                              {QVariant::Int, QVariant::Invalid},
                                              {}
                                          })) {
                return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}};
            }

            SIGlobalPropertyID id(frame.parameters()[0].value<QString>());
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIBluetoothProtocolFrame::PROPERTY_READ, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }
            if (!property.isFlagSet(SIPropertyFlag::Writeable)) {
                return {SIBluetoothProtocolFrame::PROPERTY_WRITTEN, {(int)SIStatus::Error, frame.parameters()[0]}};
            }

            QVariant value = frame.parameters()[2];

            SIPropertyWriteFlags writeFlags = SIPropertyWriteFlag::Default;
            if (frame.parameters()[1].canConvert<unsigned int>()) {
                writeFlags = static_cast<SIPropertyWriteFlags>(frame.parameters()[1].value<unsigned int>());
            }

            auto* operation = context.deviceAccessManager().writeProperty(id, value, writeFlags);
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::writePropertyOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::SUBSCRIBE_PROPERTY: {
            if (!frame.validateParameters({{QVariant::String}})) { return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}}; }

            SIGlobalPropertyID id(frame.parameters()[0].value<QString>());
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }
            if (!property.isFlagSet(SIPropertyFlag::Readable)) {
                return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {(int)SIStatus::Error, frame.parameters()[0]}};
            }

            bool status = context.deviceAccessManager().subscribeToProperty(id, this);
            return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {(int)(status ? SIStatus::Success : SIStatus::Error), frame.parameters()[0]}};
        }

        case SIBluetoothProtocolFrame::UNSUBSCRIBE_PROPERTY: {
            if (!frame.validateParameters({{QVariant::String}})) { return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}}; }

            auto id = SIGlobalPropertyID(frame.parameters()[0].toString());
            if (!id.isValid()) {
                return {SIBluetoothProtocolFrame::PROPERTY_UNSUBSCRIBED, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }

            bool status = context.deviceAccessManager().unsubscribeFromProperty(id, this);
            return {SIBluetoothProtocolFrame::PROPERTY_UNSUBSCRIBED, {(int)(status ? SIStatus::Success : SIStatus::Error), frame.parameters()[0]}};
        }

        default:
            return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}};
    }
}

SIBluetoothProtocolFrame SIBluetoothProtocolV1::convertDeviceMessage(const SIDeviceMessage& message) {
    return {SIBluetoothProtocolFrame::DEVICE_MESSAGE, {message.timestamp().toUTC().toSecsSinceEpoch(), message.accessID(), message.deviceID(), message.messageID(), message.message()}};
}

void SIBluetoothProtocolV1::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_UPDATE, {id.toString(), value}});
}

void SIBluetoothProtocolV1::enumerationOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIDeviceEnumerationOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::ENUMERATED, {(int)status, operation->numberOfDevicesPresent()}});
    operation->deleteLater();
}

void SIBluetoothProtocolV1::readPropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyReadOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_READ, {(int)status, operation->id().toString(), operation->value()}});
    operation->deleteLater();
}

void SIBluetoothProtocolV1::writePropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyWriteOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_WRITTEN, {(int)status, operation->id().toString()}});
    operation->deleteLater();
}
