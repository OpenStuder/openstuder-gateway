#include "xcom485ibatterymanager.h"
#include "xcom485imodbusaccess.h"
#include <cmath>

using namespace std;

XCom485iBatteryManager::XCom485iBatteryManager(Model model, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), 61, modbusAccess, {
    {0, 7000, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery voltage", "Vdc"},
    {2, 7001, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery current", "Adc"}
}), model_(model) {
    switch (model) {
        case Invalid:
            break;

        case BSP:
            addProperties({
                {2, 6001, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal capacity", "Ah"},
                {4, 6002, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal discharge duration (C-rating)", "h"},

                {110, 6055, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Manufacturer SOC for 0% displayed", "%"},
                {112, 6056, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Manufacturer SOC for 100% displayed", "%"}
            });
            break;

        case XComCAN_BMS:
            addProperties({
                {140, 6070, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "SOC level under which battery discharge is stopped", "%"},
                {124, 6062, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "SOC level for backup", "%"}
            });
            break;
    }
}

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
        case BSP:
            return "BSP";

        case XComCAN_BMS:
            return "XCom-Can BMS";

        case Invalid:
        default:
        return "Invalid BSP/XCom-CAN BMS model";
    }
}
