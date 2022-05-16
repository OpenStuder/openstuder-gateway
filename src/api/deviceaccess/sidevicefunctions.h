#pragma once
#include <qglobal.h>

enum class SIDeviceFunction: unsigned int {
    None = 0,
    Inverter = 1,
    Charger = 2,
    Solar = 4,
    Transfer = 8,
    Battery = 16,
    All = Inverter | Charger | Solar | Transfer | Battery
};

Q_DECLARE_FLAGS(SIDeviceFunctions, SIDeviceFunction)
Q_DECLARE_OPERATORS_FOR_FLAGS(SIDeviceFunctions)
