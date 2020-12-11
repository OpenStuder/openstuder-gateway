#include "sipropertywriteoperation.h"
#include <sideviceaccessregistry.h>
#include <sideviceaccess.h>
#include <sidevice.h>
#include <utility>

SIPropertyWriteOperation::SIPropertyWriteOperation(SIGlobalPropertyID id, QVariant value, SIPropertyWriteFlags flags, QObject* parent):
    SIAbstractOperation(parent), id_(std::move(id)), value_(std::move(value)), flags_(flags) {}

SIStatus SIPropertyWriteOperation::execute_(SIDeviceAccessRegistry* deviceAccessRegistry) {
    if (deviceAccessRegistry == nullptr) {
        status_ = SIStatus::Error;
        return status_;
    }

    auto deviceAccess = deviceAccessRegistry->deviceAccess(id_.accessID());
    if (deviceAccess == nullptr) {
        status_ = SIStatus::NoDeviceAccess;
        return status_;
    }

    auto device = deviceAccess->device(id_.deviceID());
    if (device == nullptr) {
        status_ = SIStatus::NoDevice;
        return status_;
    }

    auto result = device->writeProperty(id_.propertyID(), value_, flags_);
    status_ = result.status;
    return status_;
}
