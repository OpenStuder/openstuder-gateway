#include "siwebsocketconnection.h"
#include "siwebsocketprotocolframe.h"
#include "siwebsocketprotocolv1.h"
#include "../sisettings.h"
#include <siaccesslevel.h>

SIWebSocketConnection::SIWebSocketConnection(QWebSocket* webSocket, SIDeviceAccessManager* deviceAccessManager, SIUserAuthorizer* userAuthorizer, QObject* parent)
    : QObject(parent), webSocket_(webSocket), deviceAccessManager_(deviceAccessManager), userAuthorizer_(userAuthorizer) {
    connect(webSocket_, &QWebSocket::textMessageReceived, this, &SIWebSocketConnection::onTextMessageReceived_);
    connect(webSocket_, &QWebSocket::disconnected, this, &QObject::deleteLater);
    connect(deviceAccessManager_, &SIDeviceAccessManager::deviceMessageReceived, this, &SIWebSocketConnection::onDeviceMessageReceived_);
}

SIWebSocketConnection::~SIWebSocketConnection() {
    deviceAccessManager_->unsubscribeFromAllProperties(protocol_);
    delete protocol_;
}

void SIWebSocketConnection::onTextMessageReceived_(const QString& message) {
    SIWebSocketProtocolFrame frame = SIWebSocketProtocolFrame::fromMessage(message);

    if (protocol_ == nullptr) {
        if (frame.command() == SIWebSocketProtocolFrame::AUTHORIZE) {
            if (!frame.validateHeaders({}, {"user", "password", "protocol_version"}) || frame.hasBody()) {
                sendFrame_(SIWebSocketProtocolFrame::error("invalid frame"));
                return;
            } else {
                auto accessLevel = SIAccessLevel::None;

                if (SISettings::sharedSettings().authorizeEnabled() && frame.hasHeader("user") && frame.hasHeader("password")) {
                    auto user = frame.header("user");
                    auto pass = frame.header("password");

                    if (userAuthorizer_ != nullptr) {
                        accessLevel = userAuthorizer_->authorizeUser(user, pass);
                    }
                } else {
                    accessLevel = SISettings::sharedSettings().authorizeGuestAccessLevel();
                }

                auto versionString = frame.header("protocol_version", "1");

                if (accessLevel == SIAccessLevel::None) {
                    sendFrame_(SIWebSocketProtocolFrame::error("authorization failed"));
                    return;
                }

                bool conversionOk = false;
                int version = versionString.toInt(&conversionOk);
                if (!conversionOk) {
                    sendFrame_(SIWebSocketProtocolFrame::error("invalid version"));
                    return;
                }

                switch (version) {
                    case 1:
                        protocol_ = new SIWebSocketProtocolV1(accessLevel);
                        connect(protocol_, &SIAbstractWebSocketProtocol::frameReadyToSend, this, &SIWebSocketConnection::sendFrame_);
                        sendFrame_({SIWebSocketProtocolFrame::AUTHORIZED, {
                            {"access_level", to_string(accessLevel)},
                            {"protocol_version", QString::number(version)}
                        }});
                        break;

                    default:
                        sendFrame_(SIWebSocketProtocolFrame::error("protocol version not supported"));
                        return;
                }
            }
        } else {
            sendFrame_(SIWebSocketProtocolFrame::error("authorization required"));
        }
    } else {
        auto response = protocol_->handleFrame(frame, deviceAccessManager_);
        if (response.command() != SIWebSocketProtocolFrame::INVALID) {
            sendFrame_(response);
        }
    }
}

void SIWebSocketConnection::onDeviceMessageReceived_(const QString& deviceAccessID, const SIDeviceMessage& message) {
    if (protocol_ != nullptr) {
        sendFrame_(protocol_->convertDeviceMessage(deviceAccessID, message));
    }
}

void SIWebSocketConnection::sendFrame_(const SIWebSocketProtocolFrame& frame) {
    webSocket_->sendTextMessage(frame.toMessage());
}
