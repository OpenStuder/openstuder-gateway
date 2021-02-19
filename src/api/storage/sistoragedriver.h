#pragma once
#include "sistorage.h"
#include <QVariantMap>
#include <QStringList>
#include <QString>

class SIStorageDriver {
  public:
    SIStorageDriver() = default;
    SIStorageDriver(const SIStorageDriver&) = delete;
    SIStorageDriver& operator =(const SIStorageDriver&) = delete;
    virtual ~SIStorageDriver() = default;

    virtual SIStorage* createStorageInstance(const QVariantMap& parameters) = 0;

    static SIStorageDriver* loadStorageDriver(const QStringList& driverSearchPaths, QString driverName);
};

#define SIStorageDriver_IID "org.sgw.SIStorageDriver"
Q_DECLARE_INTERFACE(SIStorageDriver, SIStorageDriver_IID)
