#include "sidatalogmanager.h"
#include <sistorage.h>
#include "../deviceaccess/sideviceaccessmanager.h"

SIDataLogManager::SIDataLogManager(const SIDataLogConfiguration& configuration, SIContext* context, QObject* parent): QObject(parent), properties_(configuration.properties()), context_(context) {
    connect(&context_->deviceAccessManager(), &SIDeviceAccessManager::deviceMessageReceived, this, &SIDataLogManager::onDeviceMessageReceived_);
}

void SIDataLogManager::onDeviceMessageReceived_(const SIDeviceMessage& message) {
    context_->storage().storeDeviceMessage(message);
}
