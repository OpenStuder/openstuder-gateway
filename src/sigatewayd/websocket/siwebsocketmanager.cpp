#include "siwebsocketmanager.h"
#include "siwebsocketconnection.h"
#include <QHostAddress>

SIWebSocketManager::SIWebSocketManager(SIContext* context, QObject* parent):
    QObject(parent), server_("SIGateway", QWebSocketServer::NonSecureMode), context_(context) {
    connect(&server_, &QWebSocketServer::newConnection, this, &SIWebSocketManager::onNewConnection_);
}

bool SIWebSocketManager::listen(quint16 port) {
    return server_.listen(QHostAddress::Any, port);
}

void SIWebSocketManager::onNewConnection_() {
    while (server_.hasPendingConnections()) {
        auto* webSocket = server_.nextPendingConnection();
        new SIWebSocketConnection(webSocket, context_, this);
    }
}
