#include "sipropertyreadoperation.h"
#include <sideviceaccessregistry.h>
#include <sideviceaccess.h>
#include <sidevice.h>

void SIPropertyReadOperation::execute_(SIDeviceAccessRegistry* deviceAccessRegistry) {
    if (deviceAccessRegistry == nullptr) {
        status_ = SIStatus::Error;
        emit finished(status_);
    }

    auto deviceAccess = deviceAccessRegistry->deviceAccess(id_.accessID());
    if (deviceAccess == nullptr) {
        status_ = SIStatus::NoDeviceAccess;
        emit finished(status_);
    }

    auto device = deviceAccess->device(id_.deviceID());
    if (device == nullptr) {
        status_ = SIStatus::NoDevice;
        emit finished(status_);
    }

    auto result = device->readProperty(id_.propertyID());
    status_ = result.status;
    value_ = result.value;
    emit finished(status_);
}
