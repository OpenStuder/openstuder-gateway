#pragma once
#include "sipropertyid.h"
#include "sistatus.h"
#include <memory>

class SIPropertyWriteResult {
  public:
    SIPropertyWriteResult();
    SIPropertyWriteResult(SIPropertyID id, SIStatus status);

    SIPropertyID id() const;

    void setID(SIPropertyID id);

    SIStatus status() const;

    inline operator bool(); // NOLINT(google-explicit-constructor)

  private:
    void detach_();

    struct Private_;
    std::shared_ptr<Private_> private_;
};
