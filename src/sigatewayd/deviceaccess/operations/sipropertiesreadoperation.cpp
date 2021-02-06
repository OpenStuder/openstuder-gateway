#include "sipropertiesreadoperation.h"
#include <sideviceaccessregistry.h>
#include <sideviceaccess.h>
#include <sidevice.h>

SIPropertiesReadOperation::SIPropertiesReadOperation(const QVector<SIGlobalPropertyID>& ids, QObject* parent): SIAbstractOperation(parent) {
    operations_.reserve(ids.count());
    for (const auto& id: ids) {
        auto* operation = new SIPropertyReadOperation(id, this);
        connect(operation, &SIAbstractOperation::finished, this, &SIPropertiesReadOperation::onFinished_);
        operations_.append(operation);
    }
}

void SIPropertiesReadOperation::onFinished_(SIStatus status) {
    Q_UNUSED(status)

    if (++completedOperationCount_ == operations_.count()) {
        emit finished(SIStatus::Success);
    }
}

void SIPropertiesReadOperation::enqueue_(const SIAbstractOperation::QueueFunction& queueFunction) {
    for (auto* operation: operations_) {
        queueFunction(operation);
    }
}

SIStatus SIPropertiesReadOperation::execute_(SIDeviceAccessRegistry* deviceAccessRegistry) {
    Q_UNUSED(deviceAccessRegistry)

    return SIStatus::Error;
}
