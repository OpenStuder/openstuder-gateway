#pragma once
#include "siabstractpropertymanager.h"
#include <QQueue>

class SISequentialPropertyManager: public SIAbstractPropertyManager {
  public:
    explicit SISequentialPropertyManager(QObject* parent = nullptr): SIAbstractPropertyManager(parent) {}

    void start();

  private slots:
    void onFinish_(SIStatus status);
    void executeNext_();

  private:
    void enqueueOperation_(SIAbstractOperation* operation) override;

    QQueue<SIAbstractOperation*> operationQueue_;
    SIAbstractOperation* currentOperation_ = nullptr;
    int timerID_ = -1;
};
