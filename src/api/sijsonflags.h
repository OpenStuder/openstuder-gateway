#pragma once
#include <qglobal.h>

enum class SIJsonFlag: unsigned int {
    None = 0,
    IncludeAccessDetails = 1U << 0U,
    IncludeDeviceDetails = 1U << 1U,
    IncludeExpertProperties = 1U << 2U,
    Default = IncludeAccessDetails
};

Q_DECLARE_FLAGS(SIJsonFlags, SIJsonFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(SIJsonFlags)
