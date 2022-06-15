#pragma once
#include "siextension.h"
#include <QVariantMap>

class SIExtensionDriver {
  public:
    SIExtensionDriver() = default;
    SIExtensionDriver(const SIExtensionDriver&) = delete;
    SIExtensionDriver& operator =(const SIExtensionDriver&) = delete;
    virtual ~SIExtensionDriver() = default;

    virtual SIExtension* createExtensionInstance(const QVariantMap& parameters) = 0;

    static SIExtensionDriver* loadExtensionDriver(const QStringList& driverSearchPaths, QString driverName);
};

#define SIExtensionDriver_IID "org.sgw.SIExtensionDriver"
Q_DECLARE_INTERFACE(SIExtensionDriver, SIExtensionDriver_IID)
