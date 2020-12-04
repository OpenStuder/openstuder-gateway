#pragma once
#include "sipropertyid.h"
#include "sistatus.h"
#include <QVariant>

struct SIPropertyReadResult {
    SIPropertyID id;
    SIStatus status;
    QVariant value;

    inline operator bool() const {
        return status == SIStatus::Success;
    }
};
