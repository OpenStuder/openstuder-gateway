#include "sidatalogmanager.h"
#include "../deviceaccess/sideviceaccessmanager.h"
#include <sistorage.h>
#include <sideviceaccess.h>
#include <sideviceaccessregistry.h>

SIDataLogManager::SIDataLogManager(const SIDataLogConfiguration& configuration, SIContext* context, QObject* parent): QObject(parent), context_(context) {
    for (const auto& interval: configuration.properties().keys()) {
        auto properties = configuration.properties()[interval];
        groups_.append(new SIDataLogGroup(interval, properties, context_, this));
    }

    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceMessageReceived, this, &SIDataLogManager::onDeviceMessageReceived_);
    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceAdded, this, &SIDataLogManager::onDeviceAdded_);
    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceRemoved, this, &SIDataLogManager::onDeviceRemoved_);
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

void SIDataLogManager::onDeviceAdded_(const SIDeviceAccess& access, const SIDevice& device) {
    if (!device.isVirtual()) {
        for (const auto& group: groups_) {
            group->addWildcardPropertiesForDevice(access.id(), device.id(), device.properties());
        }
    }
}

void SIDataLogManager::onDeviceRemoved_(const SIDeviceAccess& access, const SIDevice& device) {
    if (!device.isVirtual()) {
        for (const auto& group: groups_) {
            group->removeWildcardPropertiesForDevice(access.id(), device.id());
        }
    }
}
