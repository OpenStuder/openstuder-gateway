#pragma once
#include <sistoragedriver.h>

class SQLiteStorageDriver: public QObject, public SIStorageDriver {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIStorageDriver_IID FILE "sqlitestoragedriver.json")
    Q_INTERFACES(SIStorageDriver)

  public:
    SIStorage* createStorageInstance(const QVariantMap& parameters) override;
};
