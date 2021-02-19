#pragma once
#include <QVariantMap>
#include <QStringList>
#include <QString>

class SIDeviceAccess;

class SIDeviceAccessDriver {
  public:
    SIDeviceAccessDriver() = default;
    SIDeviceAccessDriver(const SIDeviceAccessDriver&) = delete;
    SIDeviceAccessDriver& operator =(const SIDeviceAccessDriver&) = delete;
    virtual ~SIDeviceAccessDriver() = default;

    virtual SIDeviceAccess* createDeviceAccessInstance(const QString& id, const QVariantMap& parameters = {}) = 0;
};

#define SIDeviceAccessDriverPlugin_IID "org.sgw.SIDeviceAccessDriverPlugin"
Q_DECLARE_INTERFACE(SIDeviceAccessDriver, SIDeviceAccessDriverPlugin_IID)
