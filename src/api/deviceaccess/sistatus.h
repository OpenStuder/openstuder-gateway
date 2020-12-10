#pragma once

enum class SIStatus: int {
    Success = 0,
    InProgress = 1,
    Error = -1,
    NoProperty = -2,
    NoDevice = -3,
    NoDeviceAccess = -4,
    Timeout = -5,
    InvalidValue = -6
};

const char* to_string(SIStatus status);
