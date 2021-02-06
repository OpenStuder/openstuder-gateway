#pragma once
#include <QObject>
#include "sidataloggroup.h"
#include "sidatalogconfiguration.h"
#include "../sicontext.h"
#include <sidevicemessage.h>

class SIDataLogManager: public QObject {
    Q_OBJECT

  public:
    SIDataLogManager(const SIDataLogConfiguration& configuration, SIContext* context, QObject* parent = nullptr);

  public slots:
    void startPropertyPolling();
    void stopPropertyPolling();

  private slots:
    void onDeviceMessageReceived_(const SIDeviceMessage& message);

  private:
    QVector<SIDataLogGroup*> groups_;
    SIContext* context_;
};
