#include "sqlitestoragedriver.h"
#include "sqlitestorage.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(SQLite, "driver.storage.SQLite", QtWarningMsg)

SIStorage* SQLiteStorageDriver::createStorageInstance(const QVariantMap& parameters) {
    auto filename = parameters.value("file", OPENSTUDER_GATEWAY_INSTALL_PREFIX "/var/lib/openstuder/storage.sqlite").toString();

    // TODO: Add options:
    //  - Count or duration how long messages are kept.
    //  - Duration how long data log values will be saved globally.

    // TODO: Create folder.

    auto* storage = new SQLiteStorage();

    if (!storage->open(filename)) {
        delete storage;
        storage = nullptr;
    }

    return storage;
}
