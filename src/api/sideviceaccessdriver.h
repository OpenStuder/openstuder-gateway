#pragma once
#include "sideviceaccessregistry.h"
#include <QVector>
#include <QVariant>
#include <QString>

class SIDeviceAccessDriver {
  public:
    virtual ~SIDeviceAccessDriver() = default;

    virtual SIDeviceAccess* createDeviceAccessInstance(const QVariantMap& parameters = {}) = 0;
};

#define SIDeviceAccessDriverPlugin_IID "org.sgw.SIDeviceAccessDriverPlugin"
Q_DECLARE_INTERFACE(SIDeviceAccessDriver, SIDeviceAccessDriverPlugin_IID)
