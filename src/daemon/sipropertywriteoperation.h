#pragma once
#include "siabstractoperation.h"
#include "siglobalpropertyid.h"
#include <sipropertywriteflags.h>
#include <sistatus.h>
#include <QVariant>
#include <utility>



class SIPropertyWriteOperation: public SIAbstractOperation {
  Q_OBJECT

  public:
    inline SIPropertyWriteOperation(SIGlobalPropertyID id, QVariant value = {}, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default, QObject* parent = nullptr):
        SIAbstractOperation(parent), id_(std::move(id)), value_(std::move(value)), flags_(flags) {}

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
    SIPropertyWriteFlags flags_;
    SIStatus status_ = SIStatus::InProgress;
};
