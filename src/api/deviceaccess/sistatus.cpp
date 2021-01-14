#include "sistatus.h"

const char* to_string(SIStatus status) {
    switch (status) {
        case SIStatus::Success:
            return "Success";

        case SIStatus::InProgress:
            return "InProgress";

        case SIStatus::NoProperty:
            return "NoProperty";

        case SIStatus::NoDevice:
            return "NoDevice";

        case SIStatus::NoDeviceAccess:
            return "NoDeviceAccess";

        case SIStatus::Timeout:
            return "Timeout";

        case SIStatus::InvalidValue:
            return "InvalidValue";

        case SIStatus::Error:
        default:
            return "Error";
    }
}
