#pragma once
#include <QObject>
#include <QWebSocketServer>

class SIDeviceAccessManager;
class SIUserAuthorizer;

class SIWebSocketManager: public QObject {
    Q_OBJECT

  public:
    explicit SIWebSocketManager(SIDeviceAccessManager* deviceAccessManager, SIUserAuthorizer* userAuthorizer, QObject* parent = nullptr);

  public slots:
    bool listen(quint16 port);

  private slots:
    void onNewConnection_();

  private:
    QWebSocketServer server_;
    SIDeviceAccessManager* deviceAccessManager_;
    SIUserAuthorizer* userAuthorizer_;
};
