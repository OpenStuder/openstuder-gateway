#pragma once
#include <QString>

enum class SIPropertyType: int {
    Invalid = 0,
    Float = 1,
    Enum = 2,
    Bool = 3,
    TimeOfDay = 4,
    DaysOfWeek = 5,
    Signal = 6
};

const char* to_string(SIPropertyType type);
