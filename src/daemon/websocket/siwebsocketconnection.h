#pragma once
#include <QObject>
#include "../deviceaccess/sideviceaccessmanager.h"
#include <siuserauthorizer.h>
#include "siabstractwebsocketprotocol.h"
#include <QWebSocket>

  class SIWebSocketConnection: public QObject {
    Q_OBJECT

  public:
    SIWebSocketConnection(QWebSocket* webSocket, SIDeviceAccessManager* deviceAccessManager, SIUserAuthorizer* userAuthorizer, QObject* parent = nullptr);
    ~SIWebSocketConnection() override;

  private slots:
    void onTextMessageReceived_(const QString& message);
    void onDeviceMessageReceived_(const QString& deviceAccessID, const SIDeviceMessage& message);
    void sendFrame_(const SIWebSocketProtocolFrame& frame);

  private:
    QWebSocket* webSocket_;
    SIAbstractWebSocketProtocol* protocol_ = nullptr;
    SIDeviceAccessManager* deviceAccessManager_;
    SIUserAuthorizer* userAuthorizer_;
};
