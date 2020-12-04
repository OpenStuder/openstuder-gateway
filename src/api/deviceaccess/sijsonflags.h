#pragma once
#include <qglobal.h>

enum class SIJsonFlag: unsigned int {
    None = 0,
    IncludeAccessInformation = 1U << 0U,
    IncludeAccessDetails = 1U << 1U,
    IncludeDeviceDetails = 1U << 2U,
    IncludeExpertProperties = 1U << 3U,
    IncludeDriverInformation = 1U << 4U,
    Default = IncludeAccessInformation | IncludeAccessDetails
};

Q_DECLARE_FLAGS(SIJsonFlags, SIJsonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(SIJsonFlags)
