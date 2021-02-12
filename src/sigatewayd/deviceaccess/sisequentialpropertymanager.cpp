#include "sisequentialpropertymanager.h"

void SISequentialPropertyManager::onAboutToFinish_() {
    operationInProgress_ = nullptr;
    QMetaObject::invokeMethod(this, &SISequentialPropertyManager::executeNext_, Qt::QueuedConnection);
}

void SISequentialPropertyManager::executeNext_() {
    if (operationQueue_.isEmpty()) {
        return;
    }
    operationInProgress_ = operationQueue_.takeFirst();
    operationInProgress_->execute();
}

void SISequentialPropertyManager::enqueueOperation_(SIAbstractOperation* operation) {
    bool idle = operationQueue_.isEmpty() && operationInProgress_ == nullptr;
    connect(operation, &SIAbstractOperation::aboutToFinish, this, &SISequentialPropertyManager::onAboutToFinish_);
    operationQueue_.append(operation);
    if (idle) {
        QMetaObject::invokeMethod(this, &SISequentialPropertyManager::executeNext_, Qt::QueuedConnection);
    }
}
