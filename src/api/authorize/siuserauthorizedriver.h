#pragma once
#include "siuserauthorizer.h"
#include <QVariantMap>
#include <QStringList>
#include <QString>

class SIUserAuthorizeDriver {
  public:
    virtual ~SIUserAuthorizeDriver() = default;

    virtual SIUserAuthorizer* createUserAuthorizerInstance(const QVariantMap& parameters) = 0;

    static SIUserAuthorizeDriver* loadUserAuthorizeDriver(const QStringList& driverSearchPaths, QString driverName);
};

#define SIUserManagementDriver_IID "org.sgw.SIUserManagementDriver"
Q_DECLARE_INTERFACE(SIUserAuthorizeDriver, SIUserManagementDriver_IID)
