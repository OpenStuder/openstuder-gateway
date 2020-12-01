#include "xcom485ibatterymanager.h"
#include <cmath>

XCom485iBatteryManager::XCom485iBatteryManager(XCom485iBatteryManager::Model model): XCom485iDevice(toString(model), 61), model_(model) {}

XCom485iBatteryManager::Model XCom485iBatteryManager::model(XCom485iModbusAccess& access) {
    auto model = access.readInputRegister(61, 122);
    if (model.status != SIStatus::Success && !model.value.isValid()) {
        return Invalid;
    }
    if (isnan(model.value.toFloat())) {
        return BSP;
    } else {
        return XComCAN_BMS;
    }
}

QString XCom485iBatteryManager::toString(XCom485iBatteryManager::Model model) {
    switch (model) {
        case Invalid:
            return "Invalid BSP/XCom-CAN BMS model";

        case BSP:
            return "BSP";

        case XComCAN_BMS:
            return "XCom-Can BMS";
    }
}
