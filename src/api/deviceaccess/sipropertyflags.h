#pragma once
#include <qglobal.h>

enum class SIPropertyFlag: unsigned int {
    None = 0,
    Readable = 1U << 0U,
    Writeable = 1U << 1U
};

Q_DECLARE_FLAGS(SIPropertyFlags, SIPropertyFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(SIPropertyFlags)
