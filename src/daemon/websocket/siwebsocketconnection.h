#pragma once
#include <QObject>
#include "../deviceaccess/sideviceaccessmanager.h"
#include "siabstractwebsocketprotocol.h"
#include <QWebSocket>

  class SIWebSocketConnection: public QObject {
    Q_OBJECT

  public:
    SIWebSocketConnection(QWebSocket* webSocket, SIDeviceAccessManager* deviceAccessManager, QObject* parent = nullptr);
    ~SIWebSocketConnection() override;

  private slots:
    void onTextMessageReceived_(const QString& message);
    void onDeviceMessageReceived_(const QString& deviceAccessID, const SIDeviceMessage& message);
    void sendFrame_(const SIWebSocketProtocolFrame& frame);

  private:
    QWebSocket* webSocket_;
    SIAbstractWebSocketProtocol* protocol_ = nullptr;
    SIDeviceAccessManager* deviceAccessManager_;
};
