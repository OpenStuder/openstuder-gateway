#pragma once
#include "siabstractoperation.h"
#include "siglobalpropertyid.h"
#include <sistatus.h>
#include <QVariant>
#include <utility>

class SIPropertyReadOperation: public SIAbstractOperation {
    Q_OBJECT

  public:
    SIPropertyReadOperation(SIGlobalPropertyID id, QObject* parent = nullptr): SIAbstractOperation(parent), id_(std::move(id)) {}

    const SIGlobalPropertyID& id() const {
        return id_;
    }

    const QVariant& value() const {
        return value_;
    }

    SIStatus status() const {
        return status_;
    }

  private:
    void execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override;

    SIGlobalPropertyID id_;
    QVariant value_;
    SIStatus status_ = SIStatus::InProgress;
};
