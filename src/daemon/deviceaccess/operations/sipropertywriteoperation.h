#pragma once
#include "siabstractoperation.h"
#include "../siglobalpropertyid.h"
#include <sipropertywriteflags.h>
#include <sistatus.h>
#include <QVariant>

class SIPropertyWriteOperation final: public SIAbstractOperation {
  Q_OBJECT

  public:
    explicit SIPropertyWriteOperation(SIGlobalPropertyID id, QVariant value = {}, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default, QObject* parent = nullptr);

    inline const SIGlobalPropertyID& id() const {
        return id_;
    }

    inline const QVariant& value() const {
        return value_;
    }

    inline SIStatus status() const {
        return status_;
    }

  private:
    SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override;

    SIGlobalPropertyID id_;
    QVariant value_;
    SIPropertyWriteFlags flags_;
    SIStatus status_ = SIStatus::InProgress;
};
