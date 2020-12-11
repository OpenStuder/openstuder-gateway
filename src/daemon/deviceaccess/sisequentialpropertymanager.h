#pragma once
#include "sidevicepropertymanager.h"
#include <QQueue>

class SISequentialPropertyManager: public SIDeviceAccessManager {
  public:
    explicit SISequentialPropertyManager(QObject* parent = nullptr): SIDeviceAccessManager(parent) {}

  private slots:
    void onFinish_(SIStatus status);


  private:
    Q_INVOKABLE void executeNext_();
    void enqueueOperation_(SIAbstractOperation* operation) override;

    QQueue<SIAbstractOperation*> operationQueue_;
    SIAbstractOperation* currentOperation_ = nullptr;
    int timerID_ = -1;
};
