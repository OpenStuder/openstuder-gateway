#pragma once
#include <siaccesslevel.h>
#include <QString>

class SISessionContext {
  public:
    ~SISessionContext() = default;

    virtual QString username() const = 0;
    virtual SIAccessLevel accessLevel() = 0;
};
