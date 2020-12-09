#pragma once
#include <QString>
#include <QVariantMap>

class SIDeviceAccess;

class SIDeviceAccessDriver {
  public:
    virtual ~SIDeviceAccessDriver() = default;

    virtual SIDeviceAccess* createDeviceAccessInstance(const QString& id, const QVariantMap& parameters = {}) = 0;
};

#define SIDeviceAccessDriverPlugin_IID "org.sgw.SIDeviceAccessDriverPlugin"
Q_DECLARE_INTERFACE(SIDeviceAccessDriver, SIDeviceAccessDriverPlugin_IID)