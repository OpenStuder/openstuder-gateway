#pragma once
#include <qglobal.h>

enum class SIDescriptionFlag: unsigned int {
    None = 0,
    IncludeAccessInformation = 1U << 0U,
    IncludeDeviceInformation = 1U << 1U,
    IncludePropertyInformation = 1U << 2U,
    IncludeDriverInformation = 1U << 3U,
    Default = IncludeAccessInformation | IncludeDeviceInformation
};

Q_DECLARE_FLAGS(SIDescriptionFlags, SIDescriptionFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(SIDescriptionFlags)
