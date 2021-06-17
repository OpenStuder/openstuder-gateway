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

    // BASIC SETTINGS parameters.
    {108, 10054, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Block manual programming (dip-switch)", ""},
    {2, 10001, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage of the system", {
        {1, "Automatic"},
        {2, "12V"},
        {4, "24V"},
        {8, "48V"}
    }},
    {74, 10037, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Synchronisation battery cycle with Xtender", ""},
    {10, 10005, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating voltage", "Vdc"},
    {18, 10009, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption voltage", "Vdc"},
    {34, 10017, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization allowed", ""},
    {42, 10021, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization voltage", "Vdc"},
    {112, 10056, SIPropertyType::Signal, SIAccessLevel::Basic, SIPropertyFlag::Writeable, "Restore default settings", ""},
    {114, 10057, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "Restore factory settings", ""},

    // BATTERY MANAGEMENT AND CYCLE parameters.
    {4, 10002, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"},
    {668, 10334, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage", "Vdc"},
    {72, 10036, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature compensation", "mV/°C/cell"},

    // Floating phase parameters.
    {12, 10006, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force phase of floating", ""},

    // Absorption phase parameters.
    {16, 10008, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase allowed", ""},
    {20, 10010, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force absorption phase", ""},
    {22, 10011, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption duration", "min"},
    {24, 10012, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of absorption triggered by the current", ""},
    {26, 10013, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Current threshold to end absorption phase", "Adc"},

    // Equalization phase parameters.
    {36, 10018, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force equalization", ""},
    {40, 10020, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization current", "Adc"},
    {44, 10022, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization duration", "min"},
    {104, 10052, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization with fixed interval", ""},
    {50, 10025, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Days between equalizations", "days"},
    {52, 10026, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of equalization triggered by the current", ""},
    {54, 10027, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Current threshold to end equalization phase", "Adc"},
    {38, 10019, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization before absorption phase", ""},

    // New cycle parameters.
    {58, 10029, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force a new cycle", ""},
    {60, 10030, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage level 1 to start a new cycle", "Vdc"},
    {62, 10031, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period under voltage level 1 to start a new cycle", "min"},
    {64, 10032, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage level 2 to start a new cycle", "Vdc"},
    {66, 10033, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period under voltage level 2 to start a new cycle", "min"},
    {68, 10034, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Cycling restricted", ""},
    {70, 10035, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minimal delay between cycles", "hours"},
    {170, 10085, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage level", "Vdc"},
    {172, 10086, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Restart voltage level after a battery overvoltage", "Vdc"},

    // SYSTEM parameters.
    {120, 10060, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Check Earthing", {
        {1, "No control"},
        {2, "Neg bat pole earth"},
        {4, "Pos bat pole earth"},
        {8, "Floating battery"}
    }},
    {174, 10087, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Disabling of the display button", ""},

    // Remote entry (Remote ON/OFF) parameters.
    {626, 10313, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry active", {
        {1, "Closed"},
        {2, "Open"},
        {4, "Edge"}
    }},
    {628, 10314, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "ON/OFF command", ""},
    {630, 10315, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated by AUX1 state", ""},
    {632, 10316, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start equalization", ""},
    {634, 10317, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Send a message when remote entry changes state", ""},
    {150, 10075, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of MPP tracking", {
        {1, "P&O"},
        {2, "OC ratio"},
        {4, "Upv fixed"}
    }},
    {106, 10053, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Open circuit ratio -> MPP", "%"},
    {206, 10103, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV voltage fixed -> MPP", "Vdc"},
    {670, 10335, SIPropertyType::Bool, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Partial shading check", ""},
    {672, 10336, SIPropertyType::Float, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time between checks", "min"},
    {684, 10342, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack watchdog enabled (SCOM)", ""},
    {686, 10343, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack watchdog delay (SCOM)", "sec"},
    {400, 10200, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset PV energy meter", ""},
    {402, 10201, SIPropertyType::Signal, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Writeable, "Reset total produced PV energy meter", ""},
    {86, 10043, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset daily solar production meters", ""},
    {88, 10044, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset daily min-max", ""},
    {116, 10058, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Parameters saved in flash memory", ""},
    {78, 10039, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "ON of the VarioTrack", ""},
    {80, 10040, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "OFF of the VarioTrack", ""},
    {102, 10051, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset of all VarioTrack", ""},

    // AUXILIARY CONTACT 1 parameters.
    {178, 10089, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 1)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {180, 10090, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 1)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Contact activated in night mode (AUX 1) parameters.
    {186, 10093, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated in night mode (AUX 1)", ""},
    {188, 10094, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay of activation after entering night mode (AUX 1)", "min"},
    {190, 10095, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activation time for the auxiliary relay in night mode (AUX 1)", "min"},

    // Contact active with a fixed time schedule (AUX 1) parameters.
    {638, 10319, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with fixed time schedule (AUX 1)", ""},
    {640, 10320, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {642, 10321, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Contact active on event (AUX 1) parameters.
    {396, 10198, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is ON (AUX 1)", ""},
    {182, 10091, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is OFF (AUX 1)", ""},
    {616, 10308, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 1)", ""},
    {194, 10097, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage (AUX 1)", ""},
    {196, 10098, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 1)", ""},
    {198, 10099, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Earth fault (AUX 1)", ""},
    {200, 10100, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV error (48h without charge) (AUX 1)", ""},
    {204, 10102, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 1)", ""},
    {208, 10104, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 1)", ""},
    {210, 10105, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 1)", ""},
    {212, 10106, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 1)", ""},
    {214, 10107, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 1)", ""},
    {216, 10108, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 1)", ""},
    {218, 10109, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 1)", ""},

    // Contact active according to battery voltage (AUX 1) parameters.
    {222, 10111, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 1)", ""},
    {224, 10112, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 1)", "Vdc"},
    {226, 10113, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 1)", "min"},
    {228, 10114, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 1)", ""},
    {230, 10115, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 1)", "Vdc"},
    {232, 10116, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 1)", "min"},
    {234, 10117, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 1)", ""},
    {236, 10118, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 1)", "Vdc"},
    {238, 10119, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 1)", "min"},
    {240, 10120, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 1)", "Vdc"},
    {242, 10121, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 1)", "min"},
    {244, 10122, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 1)", ""},

    // Contact active according to battery temperature (AUX 1) With BSP or BTS parameters.
    {248, 10124, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 1)", ""},
    {250, 10125, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 1)", "°C"},
    {252, 10126, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 1)", "°C"},
    {254, 10127, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Only activated if the battery is not in bulk phase (AUX 1)", ""},

    // Contact active according to SOC (AUX 1) Only with BSP parameters.
    {258, 10129, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 1)", ""},
    {260, 10130, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 1)", "% SOC"},
    {262, 10131, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 1)", "hours"},
    {264, 10132, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 1)", ""},
    {266, 10133, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 1)", "%"},
    {268, 10134, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 1)", "hours"},
    {270, 10135, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 1)", ""},
    {272, 10136, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 1)", "%"},
    {274, 10137, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 1)", "hours"},
    {276, 10138, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 1)", "% SOC"},
    {278, 10139, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 1)", "hours"},
    {280, 10140, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 1)", ""},
    {282, 10141, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 1)", ""},

    // AUXILIARY CONTACT 2 parameters.
    {286, 10143, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 2)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {288, 10144, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 2)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Contact activated in night mode (AUX 2) parameters.
    {294, 10147, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated in night mode (AUX 2)", ""},
    {296, 10148, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay of activation after entering night mode (AUX 2)", "min"},
    {298, 10149, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activation time for the auxiliary relay in night mode (AUX 2)", "min"},

    // Contact active with a fixed time schedule (AUX 2) parameters.
    {646, 10323, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with fixed time schedule (AUX 2)", ""},
    {648, 10324, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {650, 10325, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Contact active on event (AUX 2) parameters.
    {398, 10199, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is ON (AUX 2)", ""},
    {290, 10145, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is OFF (AUX 2)", ""},
    {618, 10309, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 2)", ""},
    {302, 10151, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage (AUX 2)", ""},
    {304, 10152, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 2)", ""},
    {306, 10153, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Earth fault (AUX 2)", ""},
    {308, 10154, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV error (48h without charge) (AUX 2)", ""},
    {312, 10156, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 2)", ""},
    {316, 10158, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 2)", ""},
    {318, 10159, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 2)", ""},
    {320, 10160, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 2)", ""},
    {322, 10161, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 2)", ""},
    {324, 10162, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 2)", ""},
    {326, 10163, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 2)", ""},

    // Contact active according to battery voltage (AUX 2) parameters.
    {330, 10165, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 2)", ""},
    {332, 10166, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 2)", "Vdc"},
    {334, 10167, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 2)", "min"},
    {336, 10168, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 2)", ""},
    {338, 10169, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 2)", "Vdc"},
    {340, 10170, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 2)", "min"},
    {342, 10171, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 2)", ""},
    {344, 10172, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 2)", "Vdc"},
    {346, 10173, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 2)", "min"},
    {348, 10174, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 2)", "Vdc"},
    {350, 10175, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 2)", "min"},
    {352, 10176, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 2)", ""},

    // Contact active according to battery temperature (AUX 2) With BSP or BTS parameters.
    {356, 10178, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 2)", ""},
    {358, 10179, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 2)", "°C"},
    {360, 10180, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 2)", "°C"},
    {362, 10181, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Only activated if the battery is not in bulk phase (AUX 2)", ""},

    // Contact active according to SOC (AUX 2) Only with BSP parameters.
    {366, 10183, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 2)", ""},
    {368, 10184, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 2)", "% SOC"},
    {370, 10185, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 2)", "hours"},
    {372, 10186, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 2)", ""},
    {374, 10187, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 2)", "%"},
    {376, 10188, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 2)", "hours"},
    {378, 10189, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 2)", ""},
    {380, 10190, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 2)", "%"},
    {382, 10191, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 2)", "hours"},
    {384, 10192, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 2)", "% SOC"},
    {386, 10193, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 2)", "hours"},
    {388, 10194, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 2)", ""},
    {390, 10195, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 2)", ""},

    // AUXILIARY CONTACT 3 parameters.
    {406, 10203, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 3)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {408, 10204, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 3)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Contact activated in night mode (AUX 3) parameters.
    {412, 10206, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated in night mode (AUX 3)", ""},
    {414, 10207, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay of activation after entering night mode (AUX 3)", "min"},
    {416, 10208, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activation time for the auxiliary relay in night mode (AUX 3)", "min"},

    // Contact active with a fixed time schedule (AUX 3) parameters.
    {654, 10327, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with fixed time schedule (AUX 3)", ""},
    {656, 10328, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 3)", ""},
    {658, 10329, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 3)", ""},

    // Contact active on event (AUX 3) parameters.
    {420, 10210, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is ON (AUX 3)", ""},
    {422, 10211, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is OFF (AUX 3)", ""},
    {620, 10310, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 3)", ""},
    {424, 10212, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage (AUX 3)", ""},
    {426, 10213, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 3)", ""},
    {428, 10214, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Earth fault (AUX 3)", ""},
    {430, 10215, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV error (48h without charge) (AUX 3)", ""},
    {432, 10216, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 3)", ""},
    {434, 10217, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 3)", ""},
    {436, 10218, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 3)", ""},
    {438, 10219, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 3)", ""},
    {440, 10220, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 3)", ""},
    {442, 10221, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 3)", ""},
    {444, 10222, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 3)", ""},

    // Contact active according to battery voltage (AUX 3) parameters.
    {448, 10224, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 3)", ""},
    {450, 10225, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 3)", "Vdc"},
    {452, 10226, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 3)", "min"},
    {454, 10227, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 3)", ""},
    {456, 10228, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 3)", "Vdc"},
    {458, 10229, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 3)", "min"},
    {460, 10230, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 3)", ""},
    {462, 10231, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 3)", "Vdc"},
    {464, 10232, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 3)", "min"},
    {466, 10233, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 3)", "Vdc"},
    {468, 10234, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 3)", "min"},
    {470, 10235, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 3)", ""},

    // Contact active according to battery temperature (AUX 3) With BSP or BTS parameters.
    {474, 10237, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 3)", ""},
    {476, 10238, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 3)", "°C"},
    {478, 10239, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 3)", "°C"},
    {480, 10240, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Only activated if the battery is not in bulk phase (AUX 3)", ""},

    // Contact active according to SOC (AUX 3) Only with BSP parameters.
    {484, 10242, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 3)", ""},
    {486, 10243, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 3)", "% SOC"},
    {488, 10244, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 3)", "hours"},
    {490, 10245, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 3)", ""},
    {492, 10246, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 3)", "%"},
    {494, 10247, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 3)", "hours"},
    {496, 10248, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 3)", ""},
    {498, 10249, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 3)", "%"},
    {500, 10250, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 3)", "hours"},
    {502, 10251, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 3)", "% SOC"},
    {504, 10252, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 3)", "hours"},
    {506, 10253, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 3)", ""},
    {508, 10254, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 3)", ""},

    // AUXILIARY CONTACT 4 parameters.
    {512, 10256, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 4)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {514, 10257, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 4)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Contact activated in night mode (AUX 4) parameters.
    {518, 10259, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated in night mode (AUX 4)", ""},
    {520, 10260, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay of activation after entering night mode (AUX 4)", "min"},
    {522, 10261, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activation time for the auxiliary relay in night mode (AUX 4)", "min"},

    // Contact active with a fixed time schedule (AUX 4) parameters.
    {662, 10331, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with fixed time schedule (AUX 4)", ""},
    {664, 10332, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 4)", ""},
    {666, 10333, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 4)", ""},

    // Contact active on event (AUX 4) parameters.
    {526, 10263, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is ON (AUX 4)", ""},
    {528, 10264, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioTrack is OFF (AUX 4)", ""},
    {622, 10311, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 4)", ""},
    {530, 10265, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage (AUX 4)", ""},
    {532, 10266, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 4)", ""},
    {534, 10267, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Earth fault (AUX 4)", ""},
    {536, 10268, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV error (48h without charge) (AUX 4)", ""},
    {538, 10269, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 4)", ""},
    {540, 10270, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 4)", ""},
    {542, 10271, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 4)", ""},
    {544, 10272, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 4)", ""},
    {546, 10273, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 4)", ""},
    {548, 10274, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 4)", ""},
    {550, 10275, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 4)", ""},

    // Contact active according to battery voltage (AUX 4) parameters.
    {554, 10277, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 4)", ""},
    {556, 10278, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 4)", "Vdc"},
    {558, 10279, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 4)", "min"},
    {560, 10280, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 4)", ""},
    {562, 10281, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 4)", "Vdc"},
    {564, 10282, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 4)", "min"},
    {566, 10283, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 4)", ""},
    {568, 10284, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 4)", "Vdc"},
    {570, 10285, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 4)", "min"},
    {572, 10286, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 4)", "Vdc"},
    {574, 10287, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 4)", "min"},
    {576, 10288, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 4)", ""},

    // Contact active according to battery temperature (AUX 4) With BSP or BTS parameters.
    {580, 10290, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 4)", ""},
    {582, 10291, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 4)", "°C"},
    {584, 10292, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 4)", "°C"},
    {586, 10293, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Only activated if the battery is not in bulk phase (AUX 4)", ""},

    // Contact active according to SOC (AUX 4) Only with BSP parameters.
    {590, 10295, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 4)", ""},
    {592, 10296, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 4)", "% SOC"},
    {594, 10297, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 4)", "hours"},
    {596, 10298, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 4)", ""},
    {598, 10299, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 4)", "%"},
    {600, 10300, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 4)", "hours"},
    {602, 10301, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 4)", ""},
    {604, 10302, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 4)", "%"},
    {606, 10303, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 4)", "hours"},
    {608, 10304, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 4)", "% SOC"},
    {610, 10305, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 4)", "hours"},
    {612, 10306, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 4)", ""},
    {614, 10307, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 4)", ""}
}, model == Multicast) {}

XCom485iVarioTrack::Model XCom485iVarioTrack::model(quint8 modbusAddress, XCom485iModbusAccess& access) {
    auto model = access.readInputRegister(modbusAddress, 30);
    if (model.status() != SIStatus::Success && !model.value().isValid()) {
        return Invalid;
    }
    switch (model.value().toInt()) {
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
