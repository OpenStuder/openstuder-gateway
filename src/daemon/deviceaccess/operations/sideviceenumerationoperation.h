#pragma once
#include "siabstractoperation.h"

class SIDeviceEnumerationOperation final: public SIAbstractOperation {
  public:
    explicit SIDeviceEnumerationOperation(QObject* parent = nullptr);

    inline int numberOfDevicesPresent() const {
        return numberOfDevicesPresent_;
    }

  private:
    SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override;

    int numberOfDevicesPresent_ = -1;
};
