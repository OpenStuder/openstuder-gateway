#include "siaccesslevel.h"

const char* to_string(SIAccessLevel accessLevel) {
    switch (accessLevel) {
        case SIAccessLevel::Basic:
            return "Basic";

        case SIAccessLevel::Installer:
            return "Installer";

        case SIAccessLevel::Expert:
            return "Expert";

        case SIAccessLevel::QualifiedServicePersonnel:
            return "QSP";

        case SIAccessLevel::None:
            return "None";
    }

    return "None";
}

SIAccessLevel SIAccessLevelFromString(const QString& string) {
    if (string == "Basic") {
        return SIAccessLevel::Basic;
    } else if (string == "Installer") {
        return SIAccessLevel::Installer;
    } else if (string == "Expert") {
        return SIAccessLevel::Expert;
    } else if (string == "QSP") {
        return SIAccessLevel::QualifiedServicePersonnel;
    } else {
        return SIAccessLevel::None;
    }
}
