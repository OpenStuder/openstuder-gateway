#include "sistatus.h"

const char* to_string(SIStatus status) {
    switch (status) {
        case SIStatus::Success:
            return "Success";

        case SIStatus::NoProperty:
            return "UnsupportedProperty";

        case SIStatus::Timeout:
            return "Timeout";

        case SIStatus::Error:
        default:
            return "Error";
    }
}
