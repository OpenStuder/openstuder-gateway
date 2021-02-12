#include "sisequentialpropertymanager.h"

void SISequentialPropertyManager::onAboutToFinish_() {
    operationQueue_.removeFirst();
    QMetaObject::invokeMethod(this, &SISequentialPropertyManager::executeNext_, Qt::QueuedConnection);
}

void SISequentialPropertyManager::executeNext_() {
    if (operationQueue_.isEmpty()) {
        return;
    }
    operationQueue_.first()->execute();
}

void SISequentialPropertyManager::enqueueOperation_(SIAbstractOperation* operation) {
    bool idle = operationQueue_.isEmpty();
    connect(operation, &SIAbstractOperation::aboutToFinish, this, &SISequentialPropertyManager::onAboutToFinish_);
    operationQueue_ << operation;
    if (idle) {
        QMetaObject::invokeMethod(this, &SISequentialPropertyManager::executeNext_, Qt::QueuedConnection);
    }
}
