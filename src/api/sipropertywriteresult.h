#pragma once
#include "sipropertyid.h"
#include "sistatus.h"

struct SIPropertyWriteResult {
    SIPropertyID id;
    SIStatus status;

    inline operator bool() {
        return status == SIStatus::Success;
    }
};
