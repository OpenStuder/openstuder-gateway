#include "siwebsocketprotocolv1.h"

SIWebSocketProtocolV1::SIWebSocketProtocolV1(SIAccessLevel accessLevel): accessLevel_(accessLevel) {}

SIWebSocketProtocolFrame SIWebSocketProtocolV1::handleFrame(SIWebSocketProtocolFrame& frame, SIDeviceAccessManager* deviceAccessManager) {
    switch (frame.command()) {
        case SIWebSocketProtocolFrame::AUTHORIZE:
            return {SIWebSocketProtocolFrame::ERROR, {
                {"reason", "invalid state"}
            }};

        case SIWebSocketProtocolFrame::ENUMERATE: {
            if (frame.headers().count() != 0 || frame.hasBody()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid request"}
                }};
            }

            auto* operation = deviceAccessManager->enumerateDevices();
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::enumerationOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::DESCRIBE: {
            // TODO: Implement.
            return {SIWebSocketProtocolFrame::ERROR, {{"reason", "not implemented"}}};
        }

        case SIWebSocketProtocolFrame::READ_PROPERTY: {
            if (!frame.validateHeaders({"id"}) || frame.hasBody()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid request"}
                }};
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            if (!id.isValid()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid id"}
                }};
            }

            auto* operation = deviceAccessManager->readProperty(id);
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::readPropertyOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::WRITE_PROPERTY: {
            if (!frame.validateHeaders({"id"}, {"value"}) || frame.hasBody()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid request"}
                }};
            }

            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            if (!id.isValid()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid id"}
                }};
            }

            auto value = frame.header("value");

            auto* operation = deviceAccessManager->writeProperty(id, value);
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::writePropertyOperationFinished_);
            break;
        }

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTY: {
            if (!frame.validateHeaders({"id"}) || frame.hasBody()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid request"}
                }};
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            if (!id.isValid()) {
                return {SIWebSocketProtocolFrame::ERROR, {
                    {"reason", "invalid id"}
                }};
            }

            deviceAccessManager->subscribeToProperty(id, this);
            return {SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED, {
                {"id", id.toString()},
                {"status", to_string(SIStatus::Success)}
            }};
        }

        default:
            return {SIWebSocketProtocolFrame::ERROR, {{"reason", "invalid command"}}};
    }

    return {};
}

SIWebSocketProtocolFrame SIWebSocketProtocolV1::convertDeviceMessage(const QString& deviceAccessID, const SIDeviceMessage& message) {
    return {SIWebSocketProtocolFrame::DEVICE_MESSAGE, {
        {"access_id", deviceAccessID},
        {"device_id", message.deviceID},
        {"message_id", QString::number(message.messageID)},
        {"message", message.message}
    }};
}

void SIWebSocketProtocolV1::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_UPDATE, {
        {"id", id.toString()},
        {"value", value.toString()}
    }});
}

void SIWebSocketProtocolV1::enumerationOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIDeviceEnumerationOperation*>(sender());
    emit frameReadyToSend({SIWebSocketProtocolFrame::ENUMERATED, {
        {"status", to_string(status)},
        {"device_count", QString::number(operation->numberOfDevicesPresent())}
    }});
    delete operation;
}

void SIWebSocketProtocolV1::readPropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyReadOperation*>(sender());
    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_READ, {
        {"id", operation->id().toString()},
        {"status", to_string(status)},
        {"value", operation->value().toString()}
    }});
    delete operation;
}

void SIWebSocketProtocolV1::writePropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyWriteOperation*>(sender());
    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
        {"id", operation->id().toString()},
        {"status", to_string(status)},
    }});
    delete operation;
}
