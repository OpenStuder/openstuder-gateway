#include "siaccesslevel.h"

const char* to_string(SIAccessLevel accessLevel) {
    switch (accessLevel) {
        case SIAccessLevel::None:
            return "None";

        case SIAccessLevel::Basic:
            return "Basic";

        case SIAccessLevel::Installer:
            return "Installer";

        case SIAccessLevel::Expert:
            return "Expert";

        case SIAccessLevel::QualifiedServicePersonnel:
            return "QSP";
    }
}
