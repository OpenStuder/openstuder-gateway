#pragma once
#include <QObject>
#include "../deviceaccess/sideviceaccessmanager.h"
#include <QWebSocket>

  class SIWebSocketConnection: public QObject, public SIDeviceAccessManager::PropertySubscriber {
    Q_OBJECT

  public:
    SIWebSocketConnection(QWebSocket* webSocket, SIDeviceAccessManager* deviceAccessManager, QObject* parent = nullptr);
    ~SIWebSocketConnection() override;

  private slots:
    void onTextMessageReceived_(const QString& message);
    void onDeviceMessageReceived_(const QString& deviceAccessID, const SIDeviceMessage& message);

  private:
    void propertyChanged(SIGlobalPropertyID id, const QVariant& value) override;

    QWebSocket* webSocket_;
    SIDeviceAccessManager* deviceAccessManager_;
};
