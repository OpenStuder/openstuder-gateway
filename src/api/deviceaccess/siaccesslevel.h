#pragma once

enum class SIAccessLevel: unsigned int {
    None = 0,
    Basic,
    Installer,
    Expert,
    QualifiedServicePersonnel
};

const char* to_string(SIAccessLevel accessLevel);