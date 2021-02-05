#include "sideviceenumerationoperation.h"
#include <sideviceaccessregistry.h>

SIDeviceEnumerationOperation::SIDeviceEnumerationOperation(QObject* parent): SIAbstractOperation(parent) {}

SIStatus SIDeviceEnumerationOperation::execute_(SIDeviceAccessRegistry* deviceAccessRegistry) {
    numberOfDevicesPresent_ = deviceAccessRegistry->enumerateDevices();
    return numberOfDevicesPresent_ >= 0 ? SIStatus::Success : SIStatus::Error;
}
