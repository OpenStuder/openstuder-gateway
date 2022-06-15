#include "webdataextensiondriver.h"
#include "webdataextension.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(webdata, "driver.storage.SQLite", QtInfoMsg)

SIExtension* WebDataExtensionDriver::createExtensionInstance(const QVariantMap& parameters) {
    auto filename = parameters.value("file", OPENSTUDER_GATEWAY_INSTALL_PREFIX "/var/lib/openstuder/webdata.sqlite").toString();

    auto* extension = new WebDataExtension();

    if (!extension->open(filename)) {
        delete extension;
        extension = nullptr;
    }

    return extension;
}
