#pragma once
#include <QObject>
#include "../sicontext.h"
#include "../deviceaccess/sideviceaccessmanager.h"
#include <siuserauthorizer.h>
#include "siabstractwebsocketprotocol.h"
#include <QWebSocket>

  class SIWebSocketConnection: public QObject {
    Q_OBJECT

  public:
    SIWebSocketConnection(QWebSocket* webSocket, SIContext* context, QObject* parent = nullptr);
    ~SIWebSocketConnection() override;

  private slots:
    void onTextMessageReceived_(const QString& message);
    void onDeviceMessageReceived_(const SIDeviceMessage& message);
    void sendFrame_(const SIWebSocketProtocolFrame& frame);

  private:
    QWebSocket* webSocket_;
    SIAbstractWebSocketProtocol* protocol_ = nullptr;
    SIContext* context_;
};
