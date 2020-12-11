#include "sisequentialpropertymanager.h"

void SISequentialPropertyManager::onFinish_(SIStatus status) {
    QMetaObject::invokeMethod(this, &SISequentialPropertyManager::executeNext_, Qt::QueuedConnection);
}

void SISequentialPropertyManager::executeNext_() {
    if (operationQueue_.isEmpty()) {
        return;
    }
    currentOperation_ = operationQueue_.takeFirst();
    currentOperation_->execute();
}

void SISequentialPropertyManager::enqueueOperation_(SIAbstractOperation* operation) {
    connect(operation, &SIAbstractOperation::finished, this, &SISequentialPropertyManager::onFinish_);
    operationQueue_ << operation;
    if (operationQueue_.count() == 1) {
        QMetaObject::invokeMethod(this, &SISequentialPropertyManager::executeNext_, Qt::QueuedConnection);
    }
}
