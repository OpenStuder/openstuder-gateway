#include "siabstractoperation.h"
#include <sideviceaccessregistry.h>

void SIAbstractOperation::execute() {
    execute_(&SIDeviceAccessRegistry::sharedRegistry());
}

void SIAbstractOperation::execute(SIDeviceAccessRegistry* deviceAccessRegistry) {
    execute_(deviceAccessRegistry);
}
