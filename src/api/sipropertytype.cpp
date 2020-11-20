#include "sipropertytype.h"

const char* to_string(SIPropertyType type) {
    switch (type) {
        case SIPropertyType::Float:
            return "Float";

        case SIPropertyType::Enum:
            return "Enum";

        case SIPropertyType::Bool:
            return "Bool";

        case SIPropertyType::TimeOfDay:
            return "TimeOfDay";

        case SIPropertyType::DaysOfWeek:
            return "DaysOfWeek";

        case SIPropertyType::Signal:
            return "Signal";

        case SIPropertyType::Invalid:
        default:
            return "Invalid";
    }
}
