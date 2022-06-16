#pragma once
#include <QString>

enum class SIAccessLevel: unsigned int {
    None = 0,
    Basic = 1,
    Installer = 2,
    Expert = 3,
    QualifiedServicePersonnel = 4
};

const char* to_string(SIAccessLevel accessLevel);
SIAccessLevel SIAccessLevelFromString(const QString& string);