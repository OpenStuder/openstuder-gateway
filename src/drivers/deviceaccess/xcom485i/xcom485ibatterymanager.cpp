#include "xcom485ibatterymanager.h"
#include "xcom485imodbusaccess.h"
#include <cmath>

using namespace std;

XCom485iBatteryManager::XCom485iBatteryManager(Model model, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), 61, modbusAccess, {
    // Infos.
    {0, 7000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage", "Vdc"},
    {2, 7001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current", "Adc"},
    {4, 7002, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of Charge", "%"},
    {6, 7003, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Power", "W"},
    {14, 7007, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Ah charged today", "Ah"},
    {16, 7008, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Ah discharged today", "Ah"},
    {18, 7009, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Ah charged yesterday", "Ah"},
    {20, 7010, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Ah discharged yesterday", "Ah"},
    {58, 7029, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature", "°C"},
    {60, 7030, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute avg)", "Vdc"},
    {62, 7031, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current (minute avg)", "Adc"},
    {64, 7032, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of Charge (minute avg)", "%"},
    {66, 7033, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature (minute avg)", "°C"},
    {94, 7047, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Manufacturer State of Charge", "%"},

    // Parameters.
    {8, 6004, SIPropertyType::Signal, SIAccessLevel::Basic, SIPropertyFlag::Writeable, "Restore default settings", ""},
    {10, 6005, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "Restore factory settings", ""}
}), model_(model) {
    switch (model) {
        case Invalid:
            break;

        case BSP:
            addProperties({
                // Infos.
                {8, 7004, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Remaining autonomy", "min"},
                {22, 7011, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total Ah charged", "kAh"},
                {24, 7012, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total Ah discharged", "kAh"},
                {26, 7013, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total time", "days"},
                {34, 7017, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Custom charge Ah counter", "Ah"},
                {36, 7018, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Custom discharge Ah counter", "Ah"},
                {38, 7019, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Custom counter duration", "h"},
                {68, 7034, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID type", ""},
                {70, 7035, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID batt voltage", "Vdc"},
                {72, 7036, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID HW", ""},
                {74, 7037, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT msb", ""},
                {76, 7038, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT lsb", ""},
                {78, 7039, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Parameter number (in code)", ""},
                {80, 7040, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Info user number", ""},
                {82, 7041, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SID", ""},
                {96, 7048, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID msb", ""},
                {98, 7049, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID lsb", ""},
                {118, 7059, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Local daily communication error counter (CAN)", ""},
                {120, 7060, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of parameters (in flash)", ""},

                // Parameters.
                {114, 6057, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage of the system:", {
                    {1, "Automatic"},
                    {2, "12V"},
                    {4, "24V"},
                    {8, "48V"}
                }},
                {2, 6001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal capacity", "Ah"},
                {4, 6002, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal discharge duration (C-rating)", "h"},
                {34, 6017, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal shunt current", "A"},
                {36, 6018, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal shunt voltage", "mV"},
                {6, 6003, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset of battery history", ""},
                {62, 6031, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset of user counters", ""},
                {110, 6055, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Manufacturer SOC for 0% displayed", "%"},
                {112, 6056, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Manufacturer SOC for 100% displayed", "%"},
                {84, 6042, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activate the end of charge synchronization", ""},
                {48, 6024, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of charge voltage level", "V"},
                {50, 6025, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of charge current level", "%cap"},
                {130, 6065, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minimum duration before end of charge", "min"},
                {96, 6048, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature correction of the end of charge voltage", "mV/°C/cell"},
                {88, 6044, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activate the state of charge correction by the open circuit voltage", ""},
                {116, 6058, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current centralized regulation activated", ""},
                {118, 6059, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Max battery charge current", "A"},
                {38, 6019, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Self-discharge rate", "%/month"},
                {40, 6020, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Nominal temperature", "°C"},
                {42, 6021, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature coefficient", "%cap/°C"},
                {44, 6022, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charge efficiency factor", "%"},
                {46, 6023, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Peukert's exponent", ""},
                {98, 6049, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use C20 Capacity as reference value", ""}
            });
            break;

        case XComCAN_BMS:
            addProperties({
                // Infos.
                {106, 7053, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery Type", ""},
                {108, 7054, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "BMS Version", ""},
                {110, 7055, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery Capacity", "Ah"},
                {112, 7056, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Reserved Manufacturer ID", ""},
                {114, 7057, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State Of Health", ""},
                {116, 7058, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "High resolution manufacturer State of Charge", "%"},
                {122, 7061, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Charge voltage limit", "Vdc"},
                {124, 7062, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Discharge voltage limit", "Vdc"},
                {126, 7063, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Charge current limit", "Adc"},
                {128, 7064, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Discharge current limit", "Adc"},
                {130, 7065, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Recommended charge current", "Adc"},
                {132, 7066, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Recommended discharge current", "Adc"},

                // Parameters.
                {140, 6070, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "SOC level under which battery discharge is stopped", "%"},
                {124, 6062, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "SOC level for backup", "%"},
                {126, 6063, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "SOC level for grid feeding", "%"},
                {142, 6071, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use battery priority as energy source when SOC >= SOC for backup (not recommended in parallel)", ""},
                {136, 6068, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Allow user to define the maximum charge current of the battery", ""},
                {138, 6069, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum charge current defined by user", "A"},
                {132, 6067, SIPropertyType::Float, SIAccessLevel::Expert,SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Manufacturer SOC for 0% displayed", "%" },
                {134, 6066, SIPropertyType::Float, SIAccessLevel::Expert,SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Manufacturer SOC for 100% displayed", "%" },
                {128, 6064, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use battery current limits instead of recommended values", ""}
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
