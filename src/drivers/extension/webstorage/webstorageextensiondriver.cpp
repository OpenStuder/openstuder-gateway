#include "webstorageextensiondriver.h"
#include "webstorageextension.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(WebStorage, "driver.extension.WebStorage", QtInfoMsg)

SIExtension* WebStorageExtensionDriver::createExtensionInstance(const QVariantMap& parameters) {
    auto filename = parameters.value("file", OPENSTUDER_GATEWAY_INSTALL_PREFIX "/var/lib/openstuder/webdata.sqlite").toString();

    auto* extension = new WebStorageExtension();

    if (!extension->open(filename)) {
        delete extension;
        extension = nullptr;
    }

    return extension;
}
