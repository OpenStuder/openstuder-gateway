#include "siwebsocketconnection.h"
#include "siwebsocketprotocolframe.h"

SIWebSocketConnection::SIWebSocketConnection(QWebSocket* webSocket, SIDeviceAccessManager* deviceAccessManager, QObject* parent)
    : QObject(parent), webSocket_(webSocket), deviceAccessManager_(deviceAccessManager) {
    connect(webSocket_, &QWebSocket::textMessageReceived, this, &SIWebSocketConnection::onTextMessageReceived_);
    connect(webSocket_, &QWebSocket::disconnected, this, &QObject::deleteLater);
    connect(deviceAccessManager_, &SIDeviceAccessManager::deviceMessageReceived, this, &SIWebSocketConnection::onDeviceMessageReceived_);
}

SIWebSocketConnection::~SIWebSocketConnection() {
    deviceAccessManager_->unsubscribeFromAllProperties(this);
}

void SIWebSocketConnection::onTextMessageReceived_(const QString& message) {
    SIWebSocketProtocolFrame frame = SIWebSocketProtocolFrame::fromMessage(message);
    switch (frame.command()) {
        case SIWebSocketProtocolFrame::AUTHORIZE:
            // TODO: Version negotiation.
            break;

        case SIWebSocketProtocolFrame::ENUMERATE: {
            auto* op = deviceAccessManager_->enumerateDevices();
            connect(op, &SIAbstractOperation::finished, [this, op](SIStatus status) {
                webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
                    SIWebSocketProtocolFrame::ENUMERATED, {
                        {"status",      QString::number((int)status)},
                        {"device_count", QString::number(op->numberOfDevicesPresent())}
                    }).toMessage());
                delete op;
            });
            break;
        }

        case SIWebSocketProtocolFrame::DESCRIBE:
            break;

        case SIWebSocketProtocolFrame::READ_PROPERTY: {
            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            auto* op = deviceAccessManager_->readProperty(id);
            connect(op, &SIAbstractOperation::finished, [this, op](SIStatus status) {
                webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
                    SIWebSocketProtocolFrame::PROPERTY_READ, {
                        {"status", QString::number((int)status)},
                        {"value",  op->value().toString()}
                    }).toMessage());
                delete op;
            });
            break;
        }

        case SIWebSocketProtocolFrame::WRITE_PROPERTY: {
            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            auto value = frame.headers()["value"];
            auto* op = deviceAccessManager_->writeProperty(id, value);
            connect(op, &SIAbstractOperation::finished, [this, op](SIStatus status) {
                webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
                    SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
                        {"status", QString::number((int)status)},
                    }).toMessage());
                delete op;
            });
            break;
        }

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTY: {
            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            deviceAccessManager_->subscribeToProperty(id, this);
            webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
                SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED, {
                    {"id", id.toString()}
                }).toMessage());
            break;
        }

        default:
            webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
                SIWebSocketProtocolFrame::ERROR, {
                    {"error", "Invalid command"}
                }).toMessage());
            break;
    }
}

void SIWebSocketConnection::onDeviceMessageReceived_(const QString& deviceAccessID, const SIDeviceMessage& message) {
    webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
        SIWebSocketProtocolFrame::PROPERTY_UPDATE, {
            {"access_id", deviceAccessID},
            {"device_id",    message.deviceID},
            {"message_id", QString::number(message.messageID)},
            {"message", message.message}
        }).toMessage());
}

void SIWebSocketConnection::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    webSocket_->sendTextMessage(SIWebSocketProtocolFrame(
        SIWebSocketProtocolFrame::PROPERTY_UPDATE, {
            {"id",    id.toString()},
            {"value", value.toString()}
        }).toMessage());
}
