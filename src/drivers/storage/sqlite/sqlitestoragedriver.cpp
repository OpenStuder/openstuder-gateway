#include "sqlitestoragedriver.h"
#include "sqlitestorage.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(SQLite, "driver.storage.SQLite", QtInfoMsg)

SIStorage* SQLiteStorageDriver::createStorageInstance(const QVariantMap& parameters) {
    auto filename = parameters.value("file", OPENSTUDER_GATEWAY_INSTALL_PREFIX "/var/lib/openstuder/storage.sqlite").toString();
    auto cleanupInterval = parameters.value("cleanupInterval", 60 * 60).toInt() * 1000;
    auto maximalStorageDays = parameters.value("maxStorageDays", 2 * 730).toInt();

    auto* storage = new SQLiteStorage();

    if (!storage->open(filename, cleanupInterval, maximalStorageDays)) {
        delete storage;
        storage = nullptr;
    }

    return storage;
}
