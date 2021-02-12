#pragma once
#include "sideviceaccessmanager.h"
#include <QQueue>

class SISequentialPropertyManager: public SIDeviceAccessManager {
  public:
    explicit SISequentialPropertyManager(QObject* parent = nullptr): SIDeviceAccessManager(parent) {}

  private slots:
    void onAboutToFinish_();


  private:
    Q_INVOKABLE void executeNext_();
    void enqueueOperation_(SIAbstractOperation* operation) override;

    QQueue<SIAbstractOperation*> operationQueue_;
};
