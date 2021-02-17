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

    // BASIC SETTINGS parameters.
    {902, 1551, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Basic parameters set by means of the potentiomenter in the XTS", ""},
    {14, 1107, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum current of AC source (Input limit)", "Aac"},
    {76, 1138, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"},
    {52, 1126, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Smart-Boost allowed", ""},
    {48, 1124, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter allowed", ""},
    {50, 1125, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charger allowed", ""},
    {904, 1552, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of detection of the grid loss (AC-In)", {
        {1, "Slow"},
        {2, "Tolerant"},
        {4, "Fast"}
    }},
    {174, 1187, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Standby level", "%"},
    {590, 1395, SIPropertyType::Signal, SIAccessLevel::Basic, SIPropertyFlag::Writeable, "Restore default settings", ""},
    {374, 1287, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "Restore factory settings", ""},

    // BATTERY MANAGEMENT AND CYCLE parameters.
    {1092, 1646, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charger uses only power from AC-Out", ""},
    {78, 1139, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature compensation", "mV/°C/cell"},
    {1030, 1615, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Fast charge/inject regulation", ""},
    {1090, 1645, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Pulses cutting regulation for XT (Not XTS)", ""},

    // Undervoltage parameters.
    {16, 1108, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage level without load", "Vdc"},

    // Battery undervoltage dynamic compensation parameters.
    {182, 1191, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage dynamic compensation", ""},
    {864, 1532, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Kind of dynamic compensation", {
        {0, "Manual"},
        {1, "Automatic"}
    }},
    {1064, 1632, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Automatic adaptation of dynamic compensation", "%"},
    {18, 1109, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage level at full load", "Vdc"},
    {180, 1190, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage duration before turn off", "min"},
    {20, 1110, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Restart voltage after batteries undervoltage", "Vdc"},
    {188, 1194, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery adaptive low voltage (B.L.O)", ""},
    {190, 1195, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Max voltage for adaptive low voltage", "Vdc"},
    {414, 1307, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reset voltage for adaptive correction", "Vdc"},
    {396, 1298, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Increment step of the adaptive low voltage", "Vdc"},
    {42, 1121, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage level", "Vdc"},
    {44, 1122, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Restart voltage level after an battery overvoltage", "Vdc"},
    {80, 1140, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating voltage", "Vdc"},
    {734, 1467, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force phase of floating", ""},

    // New cycle menu parameters.
    {84, 1142, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force a new cycle", ""},
    {1016, 1608, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use dynamic compensation of battery level (new cycle)", ""},
    {86, 1143, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage level 1 to start a new cycle", "Vdc"},
    {88, 1144, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period under voltage level 1 to start a new cycle", "min"},
    {90, 1145, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage level 2 to start a new cycle", "Vdc"},
    {92, 1146, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period under voltage level 2 to start a new cycle", "sec"},
    {98, 1149, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "New cycle priority on absorption and equalization phases", ""},
    {94, 1147, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Cycling restricted", ""},
    {96, 1148, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minimal delay between cycles", "hours"},

    // Absorption phase parameters.
    {110, 1155, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase allowed", ""},
    {112, 1156, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption voltage", "Vdc"},
    {114, 1157, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption duration", "hours"},
    {116, 1158, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of absorption triggered with current", ""},
    {118, 1159, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Current limit to quit the absorption phase", "Adc"},
    {120, 1160, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximal frequency of absorption control", ""},
    {122, 1161, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minimal delay since last absorption", "hours"},

    // Equalization phase parameters.
    {126, 1163, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization allowed", ""},
    {124, 1162, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force equalization", ""},
    {382, 1291, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization before absorption phase", ""},
    {380, 1290, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization current", "Adc"},
    {128, 1164, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization voltage", "Vdc"},
    {130, 1165, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization duration", "hours"},
    {132, 1166, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Number of cycles before an equalization", ""},
    {368, 1284, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization with fixed interval", ""},
    {370, 1285, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Weeks between equalizations", "weeks"},
    {136, 1168, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of equalization triggered with current", ""},
    {138, 1169, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Current threshold to end equalization phase", "Adc"},

    // Reduced floating phase parameters.
    {140, 1170, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating allowed", ""},
    {142, 1171, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating duration before reduced floating", "days"},
    {144, 1172, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating voltage", "Vdc"},

    // Periodic absorption phase parameters.
    {146, 1173, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption allowed", ""},
    {148, 1174, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption voltage", "Vdc"},
    {150, 1175, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating duration before periodic absorption", "days"},
    {152, 1176, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption duration", "hours"},

    // INVERTER parameters.
    {372, 1286, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC Output voltage", "Vac"},
    {896, 1548, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC voltage increase according to battery voltage", ""},
    {920, 1560, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Max AC voltage increase with battery voltage", "Vac"},
    {24, 1112, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter frequency", "Hz"},
    {872, 1536, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter frequency increase when battery full", ""},
    {898, 1549, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter frequency increase according to battery voltage", ""},
    {892, 1546, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Max frequency increase", "Hz"},
    {868, 1534, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Speed of voltage or frequency change in function of battery", ""},

    // Standby and turn on parameters.
    {178, 1189, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay between standby pulses", "sec"},
    {176, 1188, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Standby number of pulses", ""},
    {998, 1599, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Softstart duration", "sec"},
    {676, 1438, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Solsafe presence Energy source at AC-Out side", ""},
    {944, 1572, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Modulator ru_soll", ""},

    // AC-IN AND TRANSFER parameters.
    {56, 1128, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Transfer relay allowed", ""},
    {960, 1580, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay before closing transfer relay", "min"},
    {1014, 1607, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Limitation of the power Boost", "%"},

    // Max input current modification parameters.
    {932, 1566, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Using a secondary value for the maximum current of the AC source", ""},
    {934, 1567, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Second maximum current of the AC source (Input limit)", "Aac"},
    {854, 1527, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Decrease max input limit current with AC-In voltage", ""},
    {908, 1554, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Decrease of the max. current of the source with input voltage activated by remote entry", ""},
    {418, 1309, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC input low limit voltage to allow charger function", "Vac"},
    {666, 1433, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Adaptation range of the input current according to the input voltage", "Vac"},
    {906, 1553, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Speed of input limit increase", ""},
    {390, 1295, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charge current decrease coef. at voltage limit to turn back in inverter mode", "%"},
    {672, 1436, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overrun AC source current limit without opening the transfer relay (Input limit)", ""},
    {820, 1510, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Tolerance on detection of AC-input loss (tolerant UPS mode)", ""},
    {198, 1199, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Input voltage giving an opening of the transfer relay with delay", "Vac"},
    {196, 1198, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay before opening of transfer relay", "sec"},
    {200, 1200, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Input voltage giving an immediate opening of the transfer relay (UPS)", "Vac"},
    {664, 1432, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absolute max limit for input voltage", "Vac"},
    {800, 1500, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Standby of the charger allowed", ""},
    {810, 1505, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delta frequency allowed above the standard input frequency", "Hz"},
    {812, 1506, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delta frequency allowed under the standard input frequency", "Hz"},
    {814, 1507, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Duration with frequency error before opening the transfer", "sec"},
    {950, 1575, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC-IN current active filtering (Not in parallel)", ""},
    {914, 1557, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use an energy quota on AC-input", ""},
    {918, 1559, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC-In energy quota", "kWh"},

    // AUXILIARY CONTACT 1 parameters.
    {204, 1202, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 1)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {794, 1497, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 1)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Temporal restrictions (AUX 1) parameters.

    // Program 1 (AUX 1) parameters.
    {210, 1205, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {212, 1206, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {214, 1207, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Program 2 (AUX 1) parameters.
    {218, 1209, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {220, 1210, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {222, 1211, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Program 3 (AUX 1) parameters.
    {226, 1213, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {228, 1214, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {230, 1215, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Program 4 (AUX 1) parameters.
    {234, 1217, SIPropertyType::DaysOfWeek, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {236, 1218, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {238, 1219, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Program 5 (AUX 1) parameters.
    {242, 1221, SIPropertyType::DaysOfWeek, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {244, 1222, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {246, 1223, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Contact active with a fixed time schedule (AUX 1) parameters.

    // Program 1 (AUX 1) parameters.
    {342, 1271, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {344, 1272, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {346, 1273, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Program 2 (AUX 1) parameters.
    {350, 1275, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {352, 1276, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {354, 1277, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Program 3 (AUX 1) parameters.
    {358, 1279, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 1)", ""},
    {360, 1280, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {362, 1281, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Contact active on event (AUX 1) parameters.
    {250, 1225, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Xtender is OFF (AUX 1)", ""},
    {836, 1518, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Xtender ON (AUX 1)", ""},
    {886, 1543, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 1)", ""},
    {252, 1226, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage alarm (AUX 1)", ""},
    {254, 1227, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 1)", ""},
    {256, 1228, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter overload (AUX 1)", ""},
    {258, 1229, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 1)", ""},
    {840, 1520, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "No overtemperature (AUX 1)", ""},
    {262, 1231, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Active charger (AUX 1)", ""},
    {264, 1232, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Active inverter (AUX 1)", ""},
    {266, 1233, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Active Smart-Boost (AUX 1)", ""},
    {268, 1234, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC input presence but with fault (AUX 1)", ""},
    {270, 1235, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC input presence (AUX 1)", ""},
    {272, 1236, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Transfer relay ON (AUX 1)", ""},
    {274, 1237, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC out presence (AUX 1)", ""},
    {276, 1238, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 1)", ""},
    {278, 1239, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 1)", ""},
    {280, 1240, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 1)", ""},
    {284, 1242, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 1)", ""},
    {286, 1243, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 1)", ""},
    {288, 1244, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 1)", ""},
    {1002, 1601, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC-In energy quota (AUX 1)", ""},

    // Contact active according to battery voltage (AUX 1) parameters.
    {376, 1288, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use dynamic compensation of battery level (AUX 1)", ""},
    {292, 1246, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 1)", ""},
    {294, 1247, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 1)", "Vdc"},
    {296, 1248, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 1)", "min"},
    {298, 1249, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 1)", ""},
    {300, 1250, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 1)", "Vdc"},
    {302, 1251, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 1)", "min"},
    {304, 1252, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 1)", ""},
    {306, 1253, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 1)", "Vdc"},
    {308, 1254, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 1)", "min"},
    {310, 1255, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 1)", "Vdc"},
    {312, 1256, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 1)", "min"},
    {832, 1516, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 1)", ""},

    // Contact active with inverter power or Smart-Boost (AUX 1) parameters.
    {316, 1258, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level 1 activate (AUX 1)", ""},
    {318, 1259, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power level 1 (AUX 1)", "% Pnom"},
    {320, 1260, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay 1 (AUX 1)", "min"},
    {1088, 1644, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated by AUX2 event partial overload", ""},
    {322, 1261, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level 2 activate (AUX 1)", ""},
    {324, 1262, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power level 2 (AUX 1)", "% Pnom"},
    {326, 1263, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay 2 (AUX 1)", "min"},
    {328, 1264, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level 3 activate (AUX 1)", ""},
    {330, 1265, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power level 3 (AUX 1)", "% Pnom"},
    {332, 1266, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay 3 (AUX 1)", "min"},
    {334, 1267, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level to deactivate (AUX 1)", "% Pnom"},
    {336, 1268, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay to deactivate (AUX 1)", "min"},

    // Contact active according to battery temperature (AUX 1) With BSP or BTS parameters.
    {692, 1446, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 1)", ""},
    {694, 1447, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 1)", "°C"},
    {696, 1448, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 1)", "°C"},

    // Contact active according to SOC (AUX 1) Only with BSP parameters.
    {678, 1439, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 1)", ""},
    {680, 1440, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 1)", "% SOC"},
    {962, 1581, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 1)", "hours"},
    {964, 1582, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 1)", ""},
    {966, 1583, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 1)", "% SOC"},
    {968, 1584, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 1)", "hours"},
    {970, 1585, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 1)", ""},
    {972, 1586, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 1)", "% SOC"},
    {974, 1587, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 1)", "hours"},
    {682, 1441, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 1)", "% SOC"},
    {976, 1588, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 1)", "hours"},
    {978, 1589, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 1)", ""},
    {824, 1512, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Security, maximum time of contact (AUX 1)", ""},
    {828, 1514, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum time of operation of contact (AUX 1)", "min"},
    {938, 1569, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 1)", ""},

    // AUXILIARY CONTACT 2 parameters.
    {422, 1311, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 2)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {796, 1498, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 2)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Temporal restrictions (AUX 2) parameters.

    // Program 1 (AUX 2) parameters.
    {428, 1314, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {430, 1315, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {432, 1316, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Program 2 (AUX 2) parameters.
    {436, 1318, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {438, 1319, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {440, 1320, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Program 3 (AUX 2) parameters.
    {444, 1322, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {446, 1323, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {448, 1324, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Program 4 (AUX 2) parameters.
    {452, 1326, SIPropertyType::DaysOfWeek, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {454, 1327, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {456, 1328, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Program 5 (AUX 2) parameters.
    {460, 1330, SIPropertyType::DaysOfWeek, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {462, 1331, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {464, 1332, SIPropertyType::TimeOfDay, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Contact active with a fixed time schedule (AUX 2) parameters.

    // Program 1 (AUX 2) parameters.
    {560, 1380, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {562, 1381, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {564, 1382, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Program 2 (AUX 2) parameters.
    {568, 1384, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {570, 1385, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {572, 1386, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Program 3 (AUX 2) parameters.
    {576, 1388, SIPropertyType::DaysOfWeek, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Day of the week (AUX 2)", ""},
    {578, 1389, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {580, 1390, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Contact active on event (AUX 2) parameters.
    {466, 1333, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Xtender is OFF (AUX 2)", ""},
    {838, 1519, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Xtender ON (AUX 2)", ""},
    {888, 1544, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 2)", ""},
    {468, 1334, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage alarm (AUX 2)", ""},
    {470, 1335, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 2)", ""},
    {472, 1336, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter overload (AUX 2)", ""},
    {474, 1337, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 2)", ""},
    {842, 1521, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "No overtemperature (AUX 2)", ""},
    {478, 1339, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Active charger (AUX 2)", ""},
    {480, 1340, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Active inverter (AUX 2)", ""},
    {482, 1341, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Active Smart-Boost (AUX 2)", ""},
    {484, 1342, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC input presence but with fault (AUX 2)", ""},
    {486, 1343, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC input presence (AUX 2)", ""},
    {488, 1344, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Transfer contact ON (AUX 2)", ""},
    {490, 1345, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC out presence (AUX 2)", ""},
    {492, 1346, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 2)", ""},
    {494, 1347, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 2)", ""},
    {496, 1348, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 2)", ""},
    {500, 1350, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 2)", ""},
    {502, 1351, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 2)", ""},
    {504, 1352, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 2)", ""},
    {1004, 1602, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "AC-In energy quota (AUX 2)", ""},
    {1086, 1643, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Partial overload", ""},

    // Contact active according to battery voltage (AUX 2) parameters.
    {508, 1354, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use dynamic compensation of battery level (AUX 2)", ""},
    {510, 1355, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 2)", ""},
    {512, 1356, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 2)", "Vdc"},
    {514, 1357, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 2)", "min"},
    {516, 1358, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 2)", ""},
    {518, 1359, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 2)", "Vdc"},
    {520, 1360, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 2)", "min"},
    {522, 1361, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 2)", ""},
    {524, 1362, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 2)", "Vdc"},
    {526, 1363, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 2)", "min"},
    {528, 1364, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 2)", "Vdc"},
    {530, 1365, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 2)", "min"},
    {834, 1517, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 2)", ""},

    // Contact active with inverter power or Smart-Boost (AUX 2) parameters.
    {534, 1367, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level 1 activate (AUX 2)", ""},
    {536, 1368, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power level 1 (AUX 2)", "% Pnom"},
    {538, 1369, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay 1 (AUX 2)", "min"},
    {540, 1370, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level 2 activate (AUX 2)", ""},
    {542, 1371, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power level 2 (AUX 2)", "% Pnom"},
    {544, 1372, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay 2 (AUX 2)", "min"},
    {546, 1373, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level 3 activate (AUX 2)", ""},
    {548, 1374, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power level 3 (AUX 2)", "% Pnom"},
    {550, 1375, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay 3 (AUX 2)", "min"},
    {552, 1376, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter power level to deactivate (AUX 2)", "% Pnom"},
    {554, 1377, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time delay to deactivate (AUX 2)", "min"},

    // Contact active according to battery temperature (AUX 2) With BSP or BTS parameters.
    {714, 1457, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 2)", ""},
    {716, 1458, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 2)", "°C"},
    {718, 1459, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 2)", "°C"},

    // Contact active according to SOC (AUX 2) Only with BSP parameters.
    {684, 1442, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 2)", ""},
    {686, 1443, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 2)", "% SOC"},
    {980, 1590, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 2)", "hours"},
    {982, 1591, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 2)", ""},
    {984, 1592, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 2)", "% SOC"},
    {986, 1593, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 2)", "hours"},
    {988, 1594, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 2)", ""},
    {990, 1595, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 2)", "% SOC"},
    {992, 1596, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 2)", "hours"},
    {688, 1444, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 2)", "% SOC"},
    {994, 1597, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 2)", "hours"},
    {996, 1598, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 2)", ""},
    {826, 1513, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Security, maximum time of contact (AUX 2)", ""},
    {830, 1515, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum time of operation of contact (AUX 2)", "min"},
    {940, 1570, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 2)", ""},

    // AUXILIARY CONTACTS 1 AND 2 EXTENDED FUNCTIONS parameters.
    {782, 1491, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Generator control active", ""},
    {786, 1493, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Number of starting attempts", ""},
    {784, 1492, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Starter pulse duration (with AUX2)", "sec"},
    {788, 1494, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time before a starter pulse", "sec"},
    {948, 1574, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Main contact hold/interrupt time", "sec"},

    // SYSTEM parameters.

    // Remote entry (Remote ON/OFF) parameters.
    {890, 1545, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry active", {
        {0, "Closed"},
        {1, "Open"}
    }},
    {876, 1538, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits transfert relay", ""},
    {878, 1539, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits inverter", ""},
    {880, 1540, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits charger", ""},
    {882, 1541, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits Smart-Boost", ""},
    {884, 1542, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits grid feeding", ""},
    {952, 1576, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "ON/OFF command", ""},
    {956, 1578, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated by AUX 1 state", ""},
    {958, 1579, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits battery priority", ""},
    {1000, 1600, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Disable minigrid mode", ""},
    {1080, 1640, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Clear AUX2 event partial overload", ""},
    {1094, 1647, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibits charger using only power from AC-Out", ""},
    {392, 1296, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Batteries priority as energy source (Not recommended in parallel)", ""},
    {394, 1297, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery priority voltage", "Vdc"},
    {930, 1565, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Buzzer alarm duration", "min"},

    // Auto restarts parameters.
    {60, 1130, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "After battery undervoltage", ""},
    {408, 1304, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Number of batteries undervoltage allowed before definitive stop", ""},
    {608, 1404, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period for batteries undervoltages counting", "sec"},
    {410, 1305, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Number of batteries critical undervoltage allowed before definitive stop", ""},
    {610, 1405, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period for critical batteries undervoltages counting", "sec"},
    {62, 1131, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "After battery overvoltage", ""},
    {64, 1132, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "After inverter or Smart-Boost overload", ""},
    {866, 1533, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to restart after an overload", "sec"},
    {68, 1134, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "After overtemperature", ""},
    {22, 1111, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Autostart to the battery connection", ""},

    // System earthing (Earth - Neutral) parameters.
    {770, 1485, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Prohibited ground relay", ""},
    {772, 1486, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Continuous neutral", ""},
    {1056, 1628, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Xtender watchdog enabled (SCOM)", ""},
    {1058, 1629, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Xtender watchdog delay (SCOM)", "sec"},
    {1032, 1616, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use of functions limited to a number of days", ""},
    {582, 1391, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Number of days without functionalitie's restrictions", "days"},
    {1034, 1617, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Transfer relay disabled after timeout", ""},
    {1036, 1618, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter disabled after timeout", ""},
    {1038, 1619, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charger disabled after timeout", ""},
    {1040, 1620, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Smart-Boost disabled after timeout", ""},
    {1042, 1621, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Grid feeding disabled after timeout", ""},
    {900, 1550, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Parameters saved in flash memory", ""},
    {630, 1415, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "ON of the Xtenders", ""},
    {598, 1399, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "OFF of the Xtenders", ""},
    {736, 1468, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset of all the inverters", ""},

    // MULTI XTENDER SYSTEM parameters.
    {366, 1283, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Integral mode", ""},
    {722, 1461, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Multi inverters allowed", ""},
    {724, 1462, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Multi inverters independents. Need reset {1468}", ""},
    {910, 1555, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery cycle synchronized by the master", ""},
    {894, 1547, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Allow slaves standby in multi-Xtender system", ""},
    {942, 1571, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Splitphase: L2 with 180 degrees phaseshift", ""},
    {916, 1558, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Separated Batteries", ""},
    {674, 1437, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minigrid compatible", ""},
    {954, 1577, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minigrid with shared battery energy", ""},
    {912, 1556, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Is the central inverter in distributed minigrid", ""},

    // GRID-FEEDING parameters.
    {54, 1127, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Grid feeding allowed", ""},
    {846, 1523, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Max grid feeding current", "Aac"},
    {848, 1524, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage target for forced grid feeding", "Vdc"},
    {850, 1525, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Forced grid feeding start time", ""},
    {852, 1526, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Forced grid feeding stop time", ""},
    {1020, 1610, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Use of the defined phase shift curve for injection", ""},
    {1044, 1622, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Cos phi at P = 0%", ""},
    {1046, 1623, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Cos phi at the power defined by param {1613}", ""},
    {1026, 1613, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Power of the second cos phi point in % of Pnom", "%"},
    {1048, 1624, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Cos phi at P = 100%", ""},
    {1054, 1627, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "ARN4105 frequency control enabled", ""},
    {1060, 1630, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delta from user frequency to start derating", "Hz"},
    {1062, 1631, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delta from user frequency to reach 100% derating", "Hz"},
    {922, 1561, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Correction for XTS saturation Reg U", ""},
    {924, 1562, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Correction for XTS saturation Reg I", ""},
    {1096, 1648, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Imagnet INT level adjustment for correction", ""},
    {1098, 1649, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Imagnet ERROR level adjustment for correction", ""}
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
