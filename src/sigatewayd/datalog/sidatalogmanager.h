#pragma once
#include <QObject>
#include "sidatalogconfiguration.h"
#include "../sicontext.h"
#include <sidevicemessage.h>

class SIDataLogManager: public QObject {
    Q_OBJECT

  public:
    SIDataLogManager(const SIDataLogConfiguration& configuration, SIContext* context, QObject* parent = nullptr);

  private slots:
    void onDeviceMessageReceived_(const SIDeviceMessage& message);

  private:
    QMap<SIGlobalPropertyID ,int> properties_;
    SIContext* context_;
};
