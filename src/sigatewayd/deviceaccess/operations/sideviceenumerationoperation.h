#pragma once
#include "siabstractoperation.h"

class SIDeviceEnumerationOperation final: public SIAbstractOperation {
    Q_OBJECT

  public:
    explicit SIDeviceEnumerationOperation(QObject* parent = nullptr);
    SIDeviceEnumerationOperation(const SIDeviceEnumerationOperation&) = delete;
    SIDeviceEnumerationOperation& operator =(const SIDeviceEnumerationOperation&) = delete;

    inline int numberOfDevicesPresent() const {
        return numberOfDevicesPresent_;
    }

  private:
    SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override;

    int numberOfDevicesPresent_ = -1;
};
