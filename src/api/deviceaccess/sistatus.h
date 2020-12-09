#pragma once

enum class SIStatus: int {
    Success = 0,
    Error = -1,
    NoProperty = -2,
    Timeout = -3,
    InvalidValue = -4
};

const char* to_string(SIStatus status);
