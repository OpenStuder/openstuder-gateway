#include "xcom485ivariotrack.h"
#include "xcom485imodbusaccess.h"

XCom485iVarioTrack::XCom485iVarioTrack(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), modbusAddress, modbusAccess,{
    // Infos.
    {0, 11000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage", "V"},
    {2, 11001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current", "A"},
    {4, 11002, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Voltage of the PV generator", "V"},
    {6, 11003, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Current of the PV generator", "A"},
    {8, 11004, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Power of the PV generator", "kW"},
    {10, 11005, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature", "°C"},
    {12, 11006, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production in (Ah) for the current day", "Ah"},
    {14, 11007, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production in (kWh) for the current day", "kWh"},
    {16, 11008, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Produced energy resettable counter", "kWh"},
    {18, 11009, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total produced energy", "MWh"},
    {20, 11010, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production in (Ah) for the previous day", "Ah"},
    {22, 11011, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production in (Wh) for the previous day", "kWh"},
    {24, 11012, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of parameters (in code)", ""},
    {26, 11013, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of parameters (in flash)", ""},
    {28, 11014, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of infos users", ""},
    {30, 11015, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Model of VarioTrack", {
        {0, "VT-80"},
        {1, "VT-65"},
        {2, "VT-40"},
        {3, "VT-HV"}
    }},
    {32, 11016, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Operating mode", {
        {0, "Night"},
        {1, "StartUp"},
        {2, "---"},
        {3, "Charger"},
        {4, "---"},
        {5, "Security"},
        {6, "OFF"},
        {7, "---"},
        {8, "Charge"},
        {9, "Charge V"},
        {10, "Charge I"},
        {11, "Charge T"},
        {12, "Ch. Ibsp"}
    }},
    {34, 11017, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV voltage for the current day", "V"},
    {36, 11018, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max battery current of the current day", "A"},
    {38, 11019, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max power production for the current day", "kW"},
    {40, 11020, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max battery voltage for the current day", "V"},
    {42, 11021, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Min battery voltage for the current day", "V"},
    {50, 11025, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of irradiation hours for the current day", "h"},
    {52, 11026, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of irradiation hours for the previous day", "h"},
    {68, 11034, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Type of error", {
        {0, "No Error"},
        {1, "BatOverV"},
        {2, "Earth"},
        {3, "No Batt"},
        {4, "OverTemp"},
        {5, "BatOverV"},
        {6, "PvOverV"},
        {7, "Others"},
        {8, "---"},
        {9, "---"},
        {10, "---"},
        {11, "---"},
        {12, "HardErr"}
    }},
    {74, 11037, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of days before next equalization", "days"},
    {76, 11038, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery cycle phase", {
        {0, "Bulk"},
        {1, "Absorpt."},
        {2, "Equalize"},
        {3, "Floating"},
        {4, "---"},
        {5, "---"},
        {6, "R.float."},
        {7, "Per.abs."},
        {8, "---"},
        {9, "---"},
        {10, "---"},
        {11, "---"}
    }},
    {78, 11039, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute avg)", "V"},
    {80, 11040, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current (minute avg)", "A"},
    {82, 11041, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV voltage (minute avg)", "V"},
    {86, 11043, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV power (minute avg)", "kW"},
    {88, 11044, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature (minute avg)", "°C"},
    {90, 11045, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute avg)", "°C"},
    {92, 11046, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute avg)", "°C"},
    {94, 11047, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID type", ""},
    {96, 11048, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID batt voltage", "V"},
    {98, 11049, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID HW", ""},
    {100, 11050, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT msb", ""},
    {102, 11051, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT lsb", ""},
    {104, 11052, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SID", ""},
    {122, 11061, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 1", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {124, 11062, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 2", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {126, 11063, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay aux 1 mode", {
        {0, "---"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"},
        {5, "G"}
    }},
    {128, 11064, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay aux 2 mode", {
        {0, "---"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"},
        {5, "G"}
    }},
    {132, 11066, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Synchronisation state", {
        {0, "---"},
        {1, "---"},
        {2, "---"},
        {3, "---"},
        {4, "XTslave"},
        {5, "VTslave"},
        {6, "---"},
        {7, "---"},
        {8, "VTmaster"},
        {9, "Autonom."},
        {10, "VSslave"},
        {11, "VSmaster"}
    }},
    {134, 11067, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID msb", ""},
    {136, 11068, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID lsb", ""},
    {138, 11069, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of the VarioTrack", {
        {0, "Off"},
        {1, "On"}
    }},
    {152, 11076, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Local daily communication error counter (CAN)", ""},
    {154, 11077, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 3", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {156, 11078, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 4", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {158, 11079, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay aux 3 mode", {
        {0, "---"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"},
        {5, "G"}
    }},
    {160, 11080, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay aux 4 mode", {
        {0, "---"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"},
        {5, "G"}
    }},
    {164, 11082, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Remote entry state", {
        {0, "RM EN 0"},
        {1, "RM EN 1"}
    }},

    // Parameters.
    {10, 10005, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating voltage", "Vdc"},
    {18, 1009, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption voltage", "Vdc"},

    {4, 10002, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"},
    {668, 10334, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage", "Vdc"}
}) {}

XCom485iVarioTrack::Model XCom485iVarioTrack::model(quint8 modbusAddress, XCom485iModbusAccess& access) {
    auto model = access.readInputRegister(modbusAddress, 30);
    if (model.status != SIStatus::Success && !model.value.isValid()) {
        return Invalid;
    }
    switch (model.value.toInt()) {
        case 0:
            return VT80;

        case 1:
            return VT65;

        default:
            return Invalid;
    }
}

QString XCom485iVarioTrack::toString(Model model) {
    switch (model) {
        case Multicast:
            return "VarioTrack multicast";

        case VT80:
            return "VarioTrack VT-80";

        case VT65:
            return "VarioTrack VT-65";

        case Invalid:
        default:
            return "Invalid VarioTrack model";
    }
}
