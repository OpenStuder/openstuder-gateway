#pragma once
#include <QString>

enum class SIExtensionStatus: int {
    Pending = 1,
    Success = 0,
    UnsupportedExtension = - 1,
    UnsupportedCommand = - 2,
    InvalidHeaders = - 3,
    InvalidParameters = InvalidHeaders,
    InvalidBody = - 4,
    Forbidden = - 5,
    Error = - 6
};

QString to_string(SIExtensionStatus status);
