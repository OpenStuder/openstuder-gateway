#include "siwebsocketconnection.h"
#include "siwebsocketprotocolframe.h"
#include "siwebsocketprotocolv1.h"
#include "../sisettings.h"
#include <siaccesslevel.h>

SIWebSocketConnection::SIWebSocketConnection(QWebSocket* webSocket, SIContext* context, QObject* parent)
    : QObject(parent), webSocket_(webSocket), context_(context) {
    connect(webSocket_, &QWebSocket::textMessageReceived, this, &SIWebSocketConnection::onTextMessageReceived_);
    connect(webSocket_, &QWebSocket::disconnected, this, &QObject::deleteLater);
    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceMessageReceived, this, &SIWebSocketConnection::onDeviceMessageReceived_);
}

SIWebSocketConnection::~SIWebSocketConnection() {
    context_->deviceAccessManager().unsubscribeFromAllProperties(protocol_);
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

                    if (context_->userAuthorizer() != nullptr) {
                        accessLevel = context_->userAuthorizer()->authorizeUser(user, pass);
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
                            {"protocol_version", QString::number(version)},
                            {"gateway_version", OPENSTUDER_GATEWAY_VERSION}
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
        auto response = protocol_->handleFrame(frame, *context_);
        if (response.command() != SIWebSocketProtocolFrame::INVALID) {
            sendFrame_(response);
        }
    }
}

void SIWebSocketConnection::onDeviceMessageReceived_(const SIDeviceMessage& message) {
    if (protocol_ != nullptr) {
        sendFrame_(protocol_->convertDeviceMessage(message));
    }
}

void SIWebSocketConnection::sendFrame_(const SIWebSocketProtocolFrame& frame) {
    webSocket_->sendTextMessage(frame.toMessage());
}
