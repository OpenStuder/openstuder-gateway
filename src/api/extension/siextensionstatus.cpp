#include "siextensionstatus.h"

QString to_string(SIExtensionStatus status) {
    switch (status) {
        case SIExtensionStatus::Pending:
            return "Pending";

        case SIExtensionStatus::Success:
            return "Success";

        case SIExtensionStatus::UnsupportedExtension:
            return "UnsupportedExtension";

        case SIExtensionStatus::UnsupportedCommand:
            return "UnsupportedCommand";

        case SIExtensionStatus::InvalidHeaders:
            return "InvalidHeaders";

        case SIExtensionStatus::InvalidBody:
            return "InvalidBody";

        case SIExtensionStatus::Forbidden:
            return "Forbidden";

        case SIExtensionStatus::Error:
            return "Error";

        default:
            return "Invalid";
    }
}