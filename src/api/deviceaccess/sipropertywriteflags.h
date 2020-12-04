#pragma once
#include <qglobal.h>

enum class SIPropertyWriteFlag: unsigned int {
    None = 0,
    Permanent = 1U << 0U,
    Default = Permanent
};

Q_DECLARE_FLAGS(SIPropertyWriteFlags, SIPropertyWriteFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(SIPropertyWriteFlags)
