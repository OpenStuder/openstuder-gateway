#pragma once
#include <QObject>
#include "../sicontext.h"
#include <QWebSocketServer>

class SIDeviceAccessManager;
class SIUserAuthorizer;

class SIWebSocketManager: public QObject {
    Q_OBJECT

  public:
    explicit SIWebSocketManager(SIContext* context, QObject* parent = nullptr);

  public slots:
    bool listen(quint16 port);

  private slots:
    void onNewConnection_();

  private:
    QWebSocketServer server_;
    SIContext* context_;
};
