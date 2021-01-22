#include "sqlitestoragedriver.h"
#include "sqlitestorage.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(SQLite, "driver.storage.SQLite", QtWarningMsg)

SIStorage* SQLiteStorageDriver::createStorageInstance(const QVariantMap& parameters) {
    auto filename = parameters.value("file", OPENSTUDER_GATEWAY_INSTALL_PREFIX "/var/lib/studergateway/storage.sqlite").toString();

    // TODO: Create folder.

    auto* storage = new SQLiteStorage();

    if (!storage->open(filename)) {
        delete storage;
        storage = nullptr;
    }

    return storage;
}
