#pragma once
#include <siaccesslevel.h>
#include <siuserauthorizer.h>
#include <QString>

class SIExtensionContext {
  public:
    virtual ~SIExtensionContext() = default;

    virtual QString sessionUserName() const = 0;
    virtual SIAccessLevel sessionAccessLevel() const = 0;

    virtual SIUserAuthorizer* userAuthorizer() const = 0;
};