#include "xcom485ivariostring.h"
#include "xcom485imodbusaccess.h"

XCom485iVarioString::XCom485iVarioString(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), modbusAddress, modbusAccess, {
    // Infos.
    {0, 15000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage", "V"},
    {2, 15001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current", "A"},
    {4, 15002, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery cycle phase", {
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
    {6, 15003, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV type of wiring", {
        {0, "Unknown"},
        {1, "Independ."},
        {2, "Series"},
        {3, "Parallel"},
        {4, "Error"}
    }},
    {8, 15004, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV voltage", "V"},
    {10, 15005, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1 voltage", "V"},
    {12, 15006, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2 voltage", "V"},
    {14, 15007, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV current", "A"},
    {16, 15008, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1 current", "A"},
    {18, 15009, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2 current", "A"},
    {20, 15010, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV power", "kW"},
    {22, 15011, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1 power", "kW"},
    {24, 15012, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2 power", "kW"},
    {26, 15013, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV operating mode", {
        {0, "Night"},
        {1, "Security"},
        {2, "OFF"},
        {3, "Charge"},
        {4, "ChargeV"},
        {5, "Charge I"},
        {6, "ChargeP"},
        {7, "ChargeIpv"},
        {8, "ChargeT"},
        {9, "---"},
        {10, "Ch.Ibsp"}
    }},
    {28, 15014, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1 operating mode", {
        {0, "Night"},
        {1, "Security"},
        {2, "OFF"},
        {3, "Charge"},
        {4, "ChargeV"},
        {5, "Charge I"},
        {6, "ChargeP"},
        {7, "ChargeIpv"},
        {8, "ChargeT"},
        {9, "---"},
        {10, "Ch.Ibsp"}
    }},
    {30, 15015, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2 operating mode", {
        {0, "Night"},
        {1, "Security"},
        {2, "OFF"},
        {3, "Charge"},
        {4, "ChargeV"},
        {5, "Charge I"},
        {6, "ChargeP"},
        {7, "ChargeIpv"},
        {8, "ChargeT"},
        {9, "---"},
        {10, "Ch.Ibsp"}
    }},
    {32, 15016, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV in (Ah) for the current day", "Ah"},
    {34, 15017, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV in (kWh) for the current day", "kWh"},
    {36, 15018, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV1 in (kWh) for the current day", "kWh"},
    {38, 15019, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV2 in (kWh) for the current day", "kWh"},
    {40, 15020, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Produced PV energy resettable counter", "kWh"},
    {42, 15021, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Produced PV1 energy resettable counter", "kWh"},
    {44, 15022, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Produced PV2 energy resettable counter", "kWh"},
    {46, 15023, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total PV produced energy", "MWh"},
    {48, 15024, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total PV1 produced energy", "MWh"},
    {50, 15025, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Total PV2 produced energy", "MWh"},
    {52, 15026, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV in (Ah) for the previous day", "Ah"},
    {54, 15027, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV in (Wh) for the previous day", "kWh"},
    {56, 15028, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV1 in (Wh) for the previous day", "kWh"},
    {58, 15029, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production PV2 in (Wh) for the previous day", "kWh"},
    {60, 15030, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of irradiation hours for the current day", "h"},
    {62, 15031, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of irradiation hours for the previous day", "h"},
    {64, 15032, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature", "°C"},
    {66, 15033, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV voltage for the current day", "V"},
    {68, 15034, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV1 voltage for the current day", "V"},
    {70, 15035, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV2 voltage for the current day", "V"},
    {72, 15036, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max battery current of the current day", "A"},
    {74, 15037, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV power for the current day", "kW"},
    {76, 15038, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV1 power for the current day", "kW"},
    {78, 15039, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max PV2 power for the current day", "kW"},
    {80, 15040, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Max battery voltage for the current day", "V"},
    {82, 15041, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Min battery voltage for the current day", "V"},
    {84, 15042, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Time in absorption of the current day", "h"},
    {86, 15043, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "BAT- and Earth voltage", "V"},
    {88, 15044, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV- and Earth voltage", "V"},
    {90, 15045, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1- and Earth voltage", "V"},
    {92, 15046, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2- and Earth voltage", "V"},
    {98, 15049, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Type of error", {
        {0, "None"},
        {1, "OverV_B"},
        {2, "OverV_PV"},
        {3, "OverV_PV1"},
        {4, "OverV_PV2"},
        {5, "OverI_PV"},
        {6, "OverI_PV1"},
        {7, "OverI_PV2"},
        {8, "GroundBat"},
        {9, "GroundPV"},
        {10, "GroundPV1"},
        {11, "GroundPV2"},
        {12, "OverTemp"},
        {13, "UnderV_B"},
        {14, "Cabling"},
        {15, "Other"}
    }},
    {100, 15050, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Synchronized with Xtender battery cycle", {
        {0, "No"},
        {1, "Yes"}
    }},
    {102, 15051, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Synchronisation state", {
        {0, "---"},
        {1, "---"},
        {2, "---"},
        {3, "---"},
        {4, "XTslave"},
        {5, "VTslave"},
        {6, "---"},
        {7, "---"},
        {8, "VTmaster"},
        {9, "Autonom"},
        {10, "VSslave"},
        {11, "VSmaster"}
    }},
    {104, 15052, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of days before next equalization", "days"},
    {106, 15053, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery set point", "V"},
    {108, 15054, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute avg)", "V"},
    {110, 15055, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute max)", "V"},
    {112, 15056, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage (minute min)", "V"},
    {114, 15057, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current (minute avg)", "A"},
    {116, 15058, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV voltage (minute avg)", "V"},
    {118, 15059, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1 voltage (minute avg)", "V"},
    {120, 15060, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2 voltage (minute avg)", "V"},
    {122, 15061, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV power (minute avg)", "kW"},
    {124, 15062, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV1 power (minute avg)", "kW"},
    {126, 15063, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "PV2 power (minute avg)", "kW"},
    {128, 15064, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature (minute avg)", "°C"},
    {130, 15065, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute avg)", "°C"},
    {132, 15066, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute max)", "°C"},
    {134, 15067, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 1 (minute min)", "°C"},
    {136, 15068, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute avg)", "°C"},
    {138, 15069, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute max)", "°C"},
    {140, 15070, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Electronic temperature 2 (minute min)", "°C"},
    {142, 15071, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of parameters (in code)", ""},
    {144, 15072, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of parameters (in flash)", ""},
    {146, 15073, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Number of infos users", ""},
    {148, 15074, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID type", ""},
    {150, 15075, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID bat voltage", "V"},
    {152, 15076, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID HW", ""},
    {154, 15077, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT msb", ""},
    {156, 15078, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SOFT lsb", ""},
    {158, 15079, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID SID", ""},
    {176, 15088, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary Aux 1", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {178, 15089, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary Aux 2", {
        {0, "Opened"},
        {1, "Closed"}
    }},
    {180, 15090, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay Aux 1 mode", {
        {0, "---"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"}
    }},
    {182, 15091, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Relay Aux 2 mode", {
        {0, "---"},
        {1, "A"},
        {2, "I"},
        {3, "M"},
        {4, "M"}
    }},
    {204, 15102, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID msb", ""},
    {206, 15103, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "ID FID lsb", ""},
    {216, 15108, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of the VarioString", {
        {0, "Off"},
        {1, "On"}
    }},
    {218, 15109, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Local daily communication error counter (CAN)", ""},
    {222, 15111, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Remote entry state", {
        {0, "RM EN 0"},
        {1, "RM EN 1"}
    }},

    // BASIC SETTINGS parameters.
    {348, 14174, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Block manual programming (dip-switch)", ""},
    {134, 14067, SIPropertyType::Signal, SIAccessLevel::Basic, SIPropertyFlag::Writeable, "Restore default settings", ""},
    {136, 14068, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "Restore factory settings", ""},

    // BATTERY MANAGEMENT AND CYCLE parameters.
    {72, 14036, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Synchronisation battery cycle with Xtender", ""},
    {432, 14216, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage", "Vdc"},
    {70, 14035, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature compensation", "mV/°C/cell"},

    // Floating phase parameters.
    {10, 14005, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating voltage", "Vdc"},
    {12, 14006, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force phase of floating", ""},

    // Absorption phase parameters.
    {16, 14008, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase allowed", ""},
    {18, 14009, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption voltage", "Vdc"},
    {20, 14010, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force absorption phase", ""},
    {22, 14011, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption duration", "min"},
    {24, 14012, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of absorption triggered by the current", ""},
    {26, 14013, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Current threshold to end absorption phase", "Adc"},

    // Equalization phase parameters.
    {34, 14017, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization allowed", ""},
    {36, 14018, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force equalization", ""},
    {42, 14021, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization voltage", "Vdc"},
    {40, 14020, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization current", "Adc"},
    {44, 14022, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization duration", "min"},
    {46, 14023, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization with fixed interval", ""},
    {48, 14024, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Days between equalizations", "days"},
    {50, 14025, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End of equalization triggered by the current", ""},
    {52, 14026, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Current threshold to end equalization phase", "Adc"},
    {38, 14019, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization before absorption phase", ""},

    // New cycle parameters.
    {56, 14028, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Force a new cycle", ""},
    {58, 14029, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage level 1 to start a new cycle", "Vdc"},
    {60, 14030, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period under voltage level 1 to start a new cycle", "min"},
    {62, 14031, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Voltage level 2 to start a new cycle", "Vdc"},
    {64, 14032, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Time period under voltage level 2 to start a new cycle", "min"},
    {66, 14033, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Cycling restricted", ""},
    {68, 14034, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Minimal delay between cycles", "hours"},
    {130, 14065, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage level", "Vdc"},
    {132, 14066, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Restart voltage level after a battery overvoltage", "Vdc"},

    // SYSTEM parameters.
    {80, 14040, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of battery grounding", {
        {1, "No control"},
        {2, "Bat+ grounded"},
        {4, "Bat- grounded"},
        {8, "Bat floating"}
    }},

    // Type of MPPT algorithm parameters.
    {384, 14192, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Establishment time (Algo MPPT)", "sec"},
    {386, 14193, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Averaging time (Algo MPPT)", "sec"},

    // Remote entry (Remote ON/OFF) parameters.
    {402, 14201, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry active", {
        {1, "Closed"},
        {2, "Open"},
        {4, "Edge"}
    }},
    {404, 14202, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "ON/OFF command", ""},
    {406, 14203, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated by AUX1 state", ""},
    {408, 14204, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start equalization", ""},
    {410, 14205, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Send a message when remote entry changes state", ""},
    {436, 14218, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioString watchdog enabled (SCOM)", ""},
    {438, 14219, SIPropertyType::Float, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioString watchdog delay (SCOM)", "sec"},
    {364, 14182, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset PV energy meter", ""},
    {366, 14183, SIPropertyType::Signal, SIAccessLevel::QualifiedServicePersonnel, SIPropertyFlag::Writeable, "Reset total produced PV energy meter", ""},
    {102, 14051, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset daily solar production meters", ""},
    {104, 14052, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset daily min-max", ""},
    {138, 14069, SIPropertyType::Bool, SIAccessLevel::Installer, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Parameters saved in flash memory", ""},
    {76, 14038, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "ON of the VarioString", ""},
    {78, 14039, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "OFF of the VarioString", ""},
    {118, 14059, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset of all VarioString", ""},

    // AUXILIARY CONTACT 1 parameters.
    {142, 14071, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 1)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {144, 14072, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 1)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Contact activated in night mode (AUX 1) parameters.
    {148, 14074, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated in night mode (AUX 1)", ""},
    {150, 14075, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay of activation after entering night mode (AUX 1)", "min"},
    {152, 14076, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activation time for the auxiliary relay in night mode (AUX 1)", "min"},

    // Contact active with a fixed time schedule (AUX 1) parameters.
    {414, 14207, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with fixed time schedule (AUX 1)", ""},
    {416, 14208, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", ""},
    {418, 14209, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 1)", ""},

    // Contact active on event (AUX 1) parameters.
    {376, 14188, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioString is ON (AUX 1)", ""},
    {156, 14078, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioString is OFF (AUX 1)", ""},
    {428, 14214, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 1)", ""},
    {158, 14079, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage (AUX 1)", ""},
    {160, 14080, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 1)", ""},
    {162, 14081, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Earth fault (AUX 1)", ""},
    {164, 14082, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV error (48h without charge) (AUX 1)", ""},
    {166, 14083, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 1)", ""},
    {168, 14084, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 1)", ""},
    {170, 14085, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 1)", ""},
    {172, 14086, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 1)", ""},
    {174, 14087, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 1)", ""},
    {176, 14088, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 1)", ""},
    {178, 14089, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 1)", ""},

    // Contact active according to battery voltage (AUX 1) parameters.
    {182, 14091, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 1)", ""},
    {184, 14092, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 1)", "Vdc"},
    {186, 14093, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 1)", "min"},
    {188, 14094, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 1)", ""},
    {190, 14095, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 1)", "Vdc"},
    {192, 14096, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 1)", "min"},
    {194, 14097, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 1)", ""},
    {196, 14098, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 1)", "Vdc"},
    {198, 14099, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 1)", "min"},
    {200, 14100, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 1)", "Vdc"},
    {202, 14101, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 1)", "min"},
    {204, 14102, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 1)", ""},

    // Contact active according to battery temperature (AUX 1) With BSP or BTS parameters.
    {208, 14104, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 1)", ""},
    {210, 14105, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 1)", "°C"},
    {212, 14106, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 1)", "°C"},
    {214, 14107, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Only activated if the battery is not in bulk phase (AUX 1)", ""},

    // Contact active according to SOC (AUX 1) Only with BSP parameters.
    {218, 14109, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 1)", ""},
    {220, 14110, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 1)", "% SOC"},
    {222, 14111, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 1)", "hours"},
    {224, 14112, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 1)", ""},
    {226, 14113, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 1)", "%"},
    {228, 14114, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 1)", "hours"},
    {230, 14115, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 1)", ""},
    {232, 14116, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 1)", "%"},
    {234, 14117, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 1)", "hours"},
    {236, 14118, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 1)", "% SOC"},
    {238, 14119, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 1)", "hours"},
    {240, 14120, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 1)", ""},
    {242, 14121, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 1)", ""},

    // AUXILIARY CONTACT 2 parameters.
    {246, 14123, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Operating mode (AUX 2)", {
        {1, "Automatic"},
        {2, "Reversed automatic"},
        {4, "Manual ON"},
        {8, "Manual OFF"}
    }},
    {248, 14124, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Combination of the events for the auxiliary contact (AUX 2)", {
        {0, "Any (Function OR)"},
        {1, "All (Function AND)"}
    }},

    // Contact activated in night mode (AUX 2) parameters.
    {252, 14126, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activated in night mode (AUX 2)", ""},
    {254, 14127, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay of activation after entering night mode (AUX 2)", "min"},
    {256, 14128, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Activation time for the auxiliary relay in night mode (AUX 2)", "min"},

    // Contact active with a fixed time schedule (AUX 2) parameters.
    {422, 14211, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with fixed time schedule (AUX 2)", ""},
    {424, 14212, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 2)", ""},
    {426, 14213, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "End hour (AUX 2)", ""},

    // Contact active on event (AUX 2) parameters.
    {378, 14189, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioString is ON (AUX 2)", ""},
    {260, 14130, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "VarioString is OFF (AUX 2)", ""},
    {430, 14215, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Remote entry (AUX 2)", ""},
    {262, 14131, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage (AUX 2)", ""},
    {264, 14132, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery overvoltage (AUX 2)", ""},
    {266, 14133, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Earth fault (AUX 2)", ""},
    {268, 14134, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV error (48h without charge) (AUX 2)", ""},
    {270, 14135, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Overtemperature (AUX 2)", ""},
    {272, 14136, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Bulk charge phase (AUX 2)", ""},
    {274, 14137, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption phase (AUX 2)", ""},
    {276, 14138, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Equalization phase (AUX 2)", ""},
    {278, 14139, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating (AUX 2)", ""},
    {280, 14140, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Reduced floating (AUX 2)", ""},
    {282, 14141, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Periodic absorption (AUX 2)", ""},

    // Contact active according to battery voltage (AUX 2) parameters.
    {286, 14143, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 activate (AUX 2)", ""},
    {288, 14144, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 1 (AUX 2)", "Vdc"},
    {290, 14145, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 2)", "min"},
    {292, 14146, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 activate (AUX 2)", ""},
    {294, 14147, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 2 (AUX 2)", "Vdc"},
    {296, 14148, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 2)", "min"},
    {298, 14149, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 activate (AUX 2)", ""},
    {300, 14150, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage 3 (AUX 2)", "Vdc"},
    {302, 14151, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 2)", "min"},
    {304, 14152, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery voltage to deactivate (AUX 2)", "Vdc"},
    {306, 14153, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 2)", "min"},
    {308, 14154, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 2)", ""},

    // Contact active according to battery temperature (AUX 2) With BSP or BTS parameters.
    {312, 14156, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the temperature of battery (AUX 2)", ""},
    {314, 14157, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated over (AUX 2)", "°C"},
    {316, 14158, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated below (AUX 2)", "°C"},
    {318, 14159, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Only activated if the battery is not in bulk phase (AUX 2)", ""},

    // Contact active according to SOC (AUX 2) Only with BSP parameters.
    {322, 14161, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 1 of battery (AUX 2)", ""},
    {324, 14162, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 1 (AUX 2)", "% SOC"},
    {326, 14163, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 1 (AUX 2)", "hours"},
    {328, 14164, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 2 of battery (AUX 2)", ""},
    {330, 14165, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 2 (AUX 2)", "%"},
    {332, 14166, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 2 (AUX 2)", "hours"},
    {334, 14167, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated with the SOC 3 of battery (AUX 2)", ""},
    {336, 14168, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact activated below SOC 3 (AUX 2)", "%"},
    {338, 14169, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay 3 (AUX 2)", "hours"},
    {340, 14170, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Contact deactivated over SOC (AUX 2)", "% SOC"},
    {342, 14171, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Delay to deactivate (AUX 2)", "hours"},
    {344, 14172, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Deactivate if battery in floating phase (AUX 2)", ""},
    {346, 14173, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "Reset all settings (AUX 2)", ""}
}, model == Multicast, SIDeviceFunction::Charger | SIDeviceFunction::Solar) {
    if (model == VS120 || model == Multicast) {
        addProperties({
            // BASIC SETTINGS parameters.
            {2, 14001, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current (VS-120)", "Adc"},
            {4, 14002, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Configuration of PV modules (VS-120)", {
                {1, "Automatic"},
                {2, "Independent"},
                {4, "Serial"},
                {8, "Parallel"}
            }},

            // Configuration for VS-120 parameters.
            {82, 14041, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of PV grounding", {
                {1, "No control"},
                {2, "PV+ grounded"},
                {4, "PV- grounded"},
                {8, "PV floating"}
            }},
            {350, 14175, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of PV1 grounding", {
                {1, "No control"},
                {2, "PV+ grounded"},
                {4, "PV- grounded"},
                {8, "PV floating"}
            }},
            {84, 14042, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of PV2 grounding", {
                {1, "No control"},
                {2, "PV+ grounded"},
                {4, "PV- grounded"},
                {8, "PV floating"}
            }},

            // Type of MPPT algorithm parameters.
            {86, 14043, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of MPP tracking algorithm PV", {
                {1, "P&O"},
                {2, "OC ratio"},
                {4, "Upv fixed"},
                {8, "LSF"}
            }},
            {88, 14044, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV voltage fixed (for PV in series)", "Vdc"},
            {358, 14179, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV voltage fixed (for PV in //)", "Vdc"},
            {90, 14045, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Ratio of PV open circuit voltage", ""},
            {352, 14176, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of MPP tracking algorithm PV1", {
                {1, "P&O"},
                {2, "OC ratio"},
                {4, "Upv fixed"},
                {8, "LSF"}
            }},
            {354, 14177, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV1 voltage fixed", "Vdc"},
            {356, 14178, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Ratio of PV1 open circuit voltage", ""},
            {92, 14046, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of MPP tracking algorithm PV2", {
                {1, "P&O"},
                {2, "OC ratio"},
                {4, "Upv fixed"},
                {8, "LSF"}
            }},
            {94, 14047, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV2 voltage fixed", "Vdc"},
            {96, 14048, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Ratio of PV2 open circuit voltage", ""},
            {380, 14190, SIPropertyType::Signal, SIAccessLevel::Installer, SIPropertyFlag::Writeable, "PV wiring type erased from memory", ""}
        });
    }

    if (model == VS70 || model == Multicast) {
        addProperties({
            // BASIC SETTINGS parameters.
            {434, 14217, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current (VS-70)", "Adc"},

            // Configuration for VS-70 parameters.
            {392, 14196, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of PV grounding", {
                {1, "No control"},
                {2, "PV+ grounded"},
                {4, "PV- grounded"},
                {8, "PV floating"}
            }},

            // Type of MPPT algorithm parameters.
            {394, 14197, SIPropertyType::Enum, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Type of MPP tracking algorithm PV", {
                {1, "P&O"},
                {2, "OC ratio"},
                {4, "Upv fixed"},
                {8, "LSF"}
            }},
            {396, 14198, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "PV voltage fixed", "Vdc"},
            {398, 14199, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Ratio of PV open circuit voltage", ""}
        });
    }
}

XCom485iVarioString::Model XCom485iVarioString::model(quint8 modbusAddress, XCom485iModbusAccess& access) {
    auto model = access.readInputRegister(modbusAddress, 148);
    if (model.status() != SIStatus::Success && !model.value().isValid()) {
        return Invalid;
    }
    switch (model.value().toInt()) {
        case 0x3201:
            return VS120;

        case 0x3301:
            return VS70;

        default:
            return Invalid;
    }
}

QString XCom485iVarioString::toString(XCom485iVarioString::Model model) {
    switch (model) {
        case Multicast:
            return "VarioString multicast";

        case VS120:
            return "VarioString VS120";

        case VS70:
            return "VarioString VS70";

        case Invalid:
        default:
        return "Invalid VarioString model";
    }
}
