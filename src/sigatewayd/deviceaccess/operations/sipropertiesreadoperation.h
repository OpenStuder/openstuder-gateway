#pragma once
#include "siabstractoperation.h"
#include "sipropertyreadoperation.h"
#include <siglobalpropertyid.h>
#include <sistatus.h>
#include <QVariant>
#include <QVector>

class SIPropertiesReadOperation final: public SIAbstractOperation {
  Q_OBJECT

  public:
    explicit SIPropertiesReadOperation(const QVector<SIGlobalPropertyID>& ids, QObject* parent = nullptr);
    SIPropertiesReadOperation(const SIPropertiesReadOperation&) = delete;
    SIPropertiesReadOperation operator =(const SIPropertiesReadOperation&) = delete;

    inline int count() const {
        return operations_.count();
    }

    inline const SIPropertyReadOperation& operator [](int index) {
        return *operations_[index];
    }

  private slots:
    void onFinished_(SIStatus status);

  private:
    void enqueue_(const QueueFunction& queueFunction) override;
    SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override;

    QVector<SIPropertyReadOperation*> operations_;
    int completedOperationCount_ = 0;
};
