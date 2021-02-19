#pragma once
#include "sipropertyid.h"
#include "sistatus.h"
#include <QVariant>
#include <memory>

class SIPropertyReadResult {
  public:
    SIPropertyReadResult();
    SIPropertyReadResult(SIPropertyID id, SIStatus status, const QVariant& value = {});

    SIPropertyID id() const;

    void setID(SIPropertyID id);

    SIStatus status() const;

    const QVariant& value() const;

    inline operator bool() const; // NOLINT(google-explicit-constructor)

  private:
    void detach_();

    struct Private_;
    std::shared_ptr<Private_> private_;
};
