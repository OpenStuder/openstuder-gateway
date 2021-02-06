#include "sidatalogmanager.h"
#include <sistorage.h>
#include "../deviceaccess/sideviceaccessmanager.h"

SIDataLogManager::SIDataLogManager(const SIDataLogConfiguration& configuration, SIContext* context, QObject* parent): QObject(parent), context_(context) {
    for (const auto& interval: configuration.properties().keys()) {
        auto properties = configuration.properties()[interval];
        groups_.append(new SIDataLogGroup(interval, properties, context_, this));
    }

    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceMessageReceived, this, &SIDataLogManager::onDeviceMessageReceived_);
}

void SIDataLogManager::startPropertyPolling() {
    for (auto* group: groups_) {
        group->startPropertyPolling();
    }
}

void SIDataLogManager::stopPropertyPolling() {
    for (auto* group: groups_) {
        group->stopPropertyPolling();
    }
}

void SIDataLogManager::onDeviceMessageReceived_(const SIDeviceMessage& message) {
    context_->storage().storeDeviceMessage(message);
}
