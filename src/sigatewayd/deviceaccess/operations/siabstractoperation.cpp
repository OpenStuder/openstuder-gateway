#include "siabstractoperation.h"
#include <sideviceaccessregistry.h>
#include <QEventLoop>

SIAbstractOperation::SIAbstractOperation(QObject* parent): QObject(parent) {}

SIAbstractOperation::~SIAbstractOperation() = default;

void SIAbstractOperation::enqueue(const std::function<void(SIAbstractOperation*)>& queueFunction) {
    enqueue_(queueFunction);
}

void SIAbstractOperation::execute() {
    auto status = execute_(&SIDeviceAccessRegistry::sharedRegistry());
    emit finished(status);
}

void SIAbstractOperation::execute(SIDeviceAccessRegistry* deviceAccessRegistry) {
    auto status = execute_(deviceAccessRegistry);
    emit finished(status);
}

void SIAbstractOperation::enqueue_(const std::function<void(SIAbstractOperation*)>& queueFunction) {
    queueFunction(this);
}
