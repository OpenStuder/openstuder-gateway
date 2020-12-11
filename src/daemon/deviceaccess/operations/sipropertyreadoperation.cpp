#include "sipropertyreadoperation.h"
#include <sideviceaccessregistry.h>
#include <sideviceaccess.h>
#include <sidevice.h>
#include <utility>

using namespace std;

SIPropertyReadOperation::SIPropertyReadOperation(SIGlobalPropertyID id, QObject* parent): SIAbstractOperation(parent), id_(move(id)) {}

SIStatus SIPropertyReadOperation::execute_(SIDeviceAccessRegistry* deviceAccessRegistry) {
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

    auto result = device->readProperty(id_.propertyID());
    status_ = result.status;
    value_ = result.value;
    return status_;
}
