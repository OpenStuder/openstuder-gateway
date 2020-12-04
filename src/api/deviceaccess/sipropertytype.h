#pragma once
#include <QString>

enum class SIPropertyType: int {
    Invalid,
    Float,
    Enum,
    Bool,
    TimeOfDay,
    DaysOfWeek,
    Signal
};

const char* to_string(SIPropertyType type);
