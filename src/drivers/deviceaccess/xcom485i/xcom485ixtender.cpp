#include "xcom485ixtender.h"
#include "xcom485imodbusaccess.h"

XCom485iXtender::XCom485iXtender(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), modbusAddress, modbusAccess, {
    // Infos.
    {0, 3000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage", "V"},
    {2, 3001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature", "°C"},
    {4, 3002, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Temperature compensation of battery voltage", "Ctmp"},
    {6, 3003, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Dynamic compensation of battery voltage", "Cdyn"},
    {8, 3004, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Wanted battery charge current", "A"},
    {10, 3005, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery charge current", "A"},
    {12, 3006, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage ripple", "V"},
    {14, 3007, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of charge", "%"},
    {16, 3008, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Low Voltage Disconect", "V"},
    {20, 3010, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery cycle phase", {
        {0, "Invalid value"},
        {1, "Bulk"},
        {2, "Absorpt."},
        {3, "Equalise"},
        {4, "Floating"},
        {5, "R.float."},
        {6, "Per.abs."},
        {7, "Mixing"},
        {8, "Forming"}
    }},
    {22, 3011, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input voltage", "V"},
    {24, 3012, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input current", "A"},
    {26, 3013, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input power", "kVA"},
    {34, 3017, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input limit value", "A"},
    {36, 3018, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input limite reached", {
        {0, "Off"},
        {1, "On"}
    }},
    {38, 3019, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Boost active", {
        {0, "Off"},
        {1, "On"}
    }},
    {40, 3020, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of transfer relay", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {42, 3021, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output voltage", "V"},
    {44, 3022, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output current", "A"},
    {46, 3023, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output power", "kVA"},
    {56, 3028, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Operating state", {
        {0, "Invalid value"},
        {1, "Inverter"},
        {2, "Charger"},
        {3, "Boost"},
        {4, "Injection"}
    }},
    {60, 3030, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of output relay", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {62, 3031, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 1", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {64, 3032, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 2", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {90, 3045, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Nbr. of overloads", ""},
    {92, 3046, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Nbr. overtemperature", ""},
    {94, 3047, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Nbr. batterie overvoltage", ""},
    {98, 3049, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of the inverter", {
        {0, "Off"},
        {1, "On"}
    }},
    {100, 3050, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of battery elements", ""},
    {102, 3051, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Search mode state", {
        {0, "Off"},
        {1, "On"}
    }},
    {108, 3054, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay aux 1 mode", {
        {0, "Invalid value"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"},
        {5, "G"}
    }},
    {110, 3055, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay aux 2 mode", {
        {0, "Invalid value"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"},
        {5, "G"}
    }},
    {112, 3056, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Lockings flag", ""},
    {148, 3074, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of the ground relay", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {150, 3075, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of the neutral transfer relay", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {152, 3076, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Discharge of battery of the previous day", "kWh"},
    {156, 3078, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Discharge of battery of the current day", "kWh"},
    {160, 3080, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Energy AC-In from the previous day", "kWh"},
    {162, 3081, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Energy AC-In from the current day", "kWh"},
    {164, 3082, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Consumers energy of the previous day", "kWh"},
    {166, 3083, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Consumers energy of the current day", "kWh"},
    {168, 3084, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input frequency", "Hz"},
    {170, 3085, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output frequency", "Hz"},
    {172, 3086, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Remote entry state", {
        {0, "RM EN 0"},
        {1, "RM EN 1"}
    }},
    {174, 3087, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output active power", "W"},
    {176, 3088, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input active power", "W"},
    {178, 3089, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Defined phase", ""},
    {180, 3090, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute min)", "V"},
    {182, 3091, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute max)", "V"},
    {184, 3092, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute avg)", "V"},
    {186, 3093, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery charge current (minute min)", "A"},
    {188, 3094, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery charge current (minute max)", "A"},
    {190, 3095, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery charge current (minute avg)", "A"},
    {192, 3096, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output power min (minute min)", "kVA"},
    {194, 3097, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output power (minute max)", "kVA"},
    {196, 3098, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output power (minute avg)", "kVA"},
    {198, 3099, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output active power (minute min)", "kW"},
    {200, 3100, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output active power (minute max)", "kW"},
    {202, 3101, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output active power (minute avg)", "kW"},
    {204, 3102, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute min)", "°C"},
    {206, 3103, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute max)", "°C"},
    {208, 3104, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute avg)", "°C"},
    {210, 3105, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute min)", "°C"},
    {212, 3106, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute max)", "°C"},
    {214, 3107, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute avg)", "°C"},
    {216, 3108, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output frequency (minute min)", "Hz"},
    {218, 3109, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output frequency (minute max)", "Hz"},
    {220, 3110, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output frequency (minute avg)", "Hz"},
    {222, 3111, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input voltage (minute min)", "V"},
    {224, 3112, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input voltage (minute max)", "V"},
    {226, 3113, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input voltage (minute avg)", "V"},
    {228, 3114, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input current (minute min)", "A"},
    {230, 3115, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input current (minute max)", "A"},
    {232, 3116, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input current (minute avg)", "A"},
    {234, 3117, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input active power (minute min)", "kW"},
    {236, 3118, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input active power (minute max)", "kW"},
    {238, 3119, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input active power (minute avg)", "kW"},
    {240, 3120, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input frequency (minute min)", "Hz"},
    {242, 3121, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input frequency (minute max)", "Hz"},
    {244, 3122, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input frequency (minute avg)", "Hz"},
    {248, 3124, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID type", ""},
    {250, 3125, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID Power", "VA"},
    {252, 3126, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID Uout", "V"},
    {254, 3127, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID batt voltage", "V"},
    {256, 3128, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID Iout nom", "A"},
    {258, 3129, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID HW", ""},
    {260, 3130, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT msb", ""},
    {262, 3131, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT lsb", ""},
    {264, 3132, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID HW PWR", ""},
    {266, 3133, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Parameter number (in code)", ""},
    {268, 3134, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Info user number", ""},
    {270, 3135, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SID", ""},
    {272, 3136, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output active power", "kW"},
    {274, 3137, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input active power", "kW"},
    {276, 3138, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input power", "kVA"},
    {278, 3139, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output power", "kVA"},
    {280, 3140, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "System debug 1", ""},
    {282, 3141, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "System debug 2", ""},
    {284, 3142, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "System state machine", ""},
    {308, 3154, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input frequency", "Hz"},
    {310, 3155, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Desired AC injection current", "A"},
    {312, 3156, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID msb", ""},
    {314, 3157, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID lsb", ""},
    {316, 3158, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Actual freezed current in ARN4105 P(f)", ""},
    {318, 3159, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "AC injection current, type of limitation ARN4105 P(f)", {
        {0, "No limit"},
        {1, "FreezeOF"},
        {2, "N_ImaxOF"},
        {3, "FreezeUF"},
        {4, "N_ImaxUF"},
        {5, "N_IMaxST"}
    }},
    {320, 3160, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Source of limitation of the functions charger or injector", {
        {0, "Invalid value"},
        {1, "Ubatt"},
        {2, "Ubattp"},
        {3, "Ubattpp"},
        {4, "Ibatt"},
        {5, "Pchar"},
        {6, "UbattInj"},
        {7, "Iinj"},
        {8, "Imax"},
        {9, "Ilim"},
        {10, "Ithermal"},
        {11, "PchNeg"},
        {12, "ARN f"}
    }},
    {322, 3161, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery priority active", {
        {0, "Off"},
        {1, "On"}
    }},
    {324, 3162, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Forced grid feeding active", {
        {0, "Off"},
        {1, "On"}
    }},
    {328, 3164, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage target for charger/injection", ""},
    {330, 3165, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Allowed charge current in limited charger mode", ""},
    {332, 3166, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Current on converter output stage DC/AC", ""},
    {334, 3167, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Voltage on converter output stage DC/AC", ""},
    {336, 3168, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Over temperature state", {
        {0, "No Error"},
        {1, "TR.Alarm"},
        {2, "TR.Error"},
        {3, "EL.Error"},
        {4, "EL.Stop"}
    }},
    {338, 3169, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "AC injection current limit ARN4105 P(f)", ""},

    // Parameters.
    {14, 1107, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum current of AC source (Input limit)", "Aac"},
    {76, 1138, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"},
    {48, 1124, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter allowed", ""},

    {50, 1125, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charger allowed", ""},
    {78, 1139, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature compensation", "mV/°C/cell"},
    {212, 1206, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", "hh:mm"},
    {630, 1415, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "ON of the Xtenders", ""},
    {598, 1399, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "OFF of the Xtenders", ""}
}) {}

XCom485iXtender::Model XCom485iXtender::model(quint8 modbusAddress, XCom485iModbusAccess& access) {
    auto model = access.readInputRegister(modbusAddress, 248);
    if (model.status != SIStatus::Success && !model.value.isValid()) {
        return Invalid;
    }
    switch (model.value.toInt()) {
        case 1:
            return XTH;

        case 256:
            return XTM;

        case 512:
            return XTS;

        default:
            return Invalid;
    }
}

QString XCom485iXtender::toString(XCom485iXtender::Model model) {
    switch (model) {
        case Multicast:
            return "Xtender multicast";

        case XTH:
            return "Xtender XTH";

        case XTM:
            return "Xtender XTM";

        case XTS:
            return "Xtender XTS";

        case Invalid:
        default:
            return "Invalid Xtender model";
    }
}
