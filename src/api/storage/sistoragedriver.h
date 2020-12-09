#pragma once
#include "sistorage.h"
#include <QVariantMap>
#include <QDateTime>

class SIStorageDriver {
  public:
    virtual ~SIStorageDriver() = default;

    virtual SIStorage* createStorageInstance(const QVariantMap& parameters = {}) = 0;
};

#define SIStorageDriver_IID "org.sgw.SIStorageDriver"
Q_DECLARE_INTERFACE(SIStorageDriver, SIStorageDriver_IID)