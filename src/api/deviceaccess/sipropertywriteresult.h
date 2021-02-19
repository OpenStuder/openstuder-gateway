#pragma once
#include "sipropertyid.h"
#include "sistatus.h"

class SIPropertyWriteResult {
  public:
    SIPropertyWriteResult();
    SIPropertyWriteResult(SIPropertyID id, SIStatus status);

    SIPropertyID id() const;

    void setID(SIPropertyID id);

    SIStatus status() const;

    inline operator bool();

  private:
    void detach_();

    struct Private_;
    std::shared_ptr<Private_> private_;
};
