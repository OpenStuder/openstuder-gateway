#include "siwebsocketconnection.h"
#include "siprotocolframe.h"

SIWebSocketConnection::SIWebSocketConnection(QWebSocket* webSocket, SIDeviceAccessManager* deviceAccessManager, QObject* parent)
    : QObject(parent), webSocket_(webSocket), deviceAccessManager_(deviceAccessManager) {
    connect(webSocket_, &QWebSocket::textMessageReceived, this, &SIWebSocketConnection::onTextMessageReceived);
    connect(webSocket_, &QWebSocket::disconnected, this, &QObject::deleteLater);
}

SIWebSocketConnection::~SIWebSocketConnection() {
    deviceAccessManager_->unsubscribeFromAllProperties(this);
}

void SIWebSocketConnection::onTextMessageReceived(const QString& message) {
    SIProtocolFrame frame = SIProtocolFrame::fromMessage(message);
    switch (frame.command()) {
        case SIProtocolFrame::AUTHORIZE:
            break;

        case SIProtocolFrame::ENUMERATE: {
            auto* op = deviceAccessManager_->enumerateDevices();
            connect(op, &SIAbstractOperation::finished, [this, op](SIStatus status) {
                webSocket_->sendTextMessage(SIProtocolFrame(
                    SIProtocolFrame::ENUMERATED, {
                        {"status",      QString::number((int)status)},
                        {"deviceCount", QString::number(op->numberOfDevicesPresent())}
                    }).toMessage());
                delete op;
            });
            break;
        }

        case SIProtocolFrame::DESCRIBE:
            break;

        case SIProtocolFrame::READ_PROPERTY: {
            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            auto* op = deviceAccessManager_->readProperty(id);
            connect(op, &SIAbstractOperation::finished, [this, op](SIStatus status) {
                webSocket_->sendTextMessage(SIProtocolFrame(
                    SIProtocolFrame::PROPERTY_READ, {
                        {"status", QString::number((int)status)},
                        {"value",  op->value().toString()}
                    }).toMessage());
                delete op;
            });
            break;
        }

        case SIProtocolFrame::WRITE_PROPERTY: {
            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            auto value = frame.headers()["value"];
            auto* op = deviceAccessManager_->writeProperty(id, value);
            connect(op, &SIAbstractOperation::finished, [this, op](SIStatus status) {
                webSocket_->sendTextMessage(SIProtocolFrame(
                    SIProtocolFrame::PROPERTY_WRITTEN, {
                        {"status", QString::number((int)status)},
                    }).toMessage());
                delete op;
            });
            break;
        }

        case SIProtocolFrame::SUBSCRIBE_PROPERTY: {
            auto id = SIGlobalPropertyID(frame.headers()["id"]);
            deviceAccessManager_->subscribeToProperty(id, this);
            webSocket_->sendTextMessage(SIProtocolFrame(
                SIProtocolFrame::PROPERTY_SUBSCRIBED, {
                    {"id", id.toString()}
                }).toMessage());
            break;
        }

        default:
            webSocket_->sendTextMessage(SIProtocolFrame(
                SIProtocolFrame::ERROR, {
                    {"error", "Invalid command"}
                }).toMessage());
            break;
    }
}

void SIWebSocketConnection::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    webSocket_->sendTextMessage(SIProtocolFrame(
        SIProtocolFrame::PROPERTY_UPDATE, {
            {"id",    id.toString()},
            {"value", value.toString()}
        }).toMessage());
}
