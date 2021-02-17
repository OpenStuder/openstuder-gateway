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

    // Parameters.
    {432, 14216, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery undervoltage", "Vdc"}
}) {
    switch (model) {
        case Invalid:
            break;
        case Multicast:
            addProperties({
                {2, 14001, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current (VS-120)", "Adc"},
                {434, 14217, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current (VS-70)", "Adc"}
            });
            break;

        case VS120:
            addProperties({
                {2, 14001, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"}
            });
            break;

        case VS70:
            addProperties({
                {434, 14217, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"}
            });
            break;
    }
}

XCom485iVarioString::Model XCom485iVarioString::model(quint8 modbusAddress, XCom485iModbusAccess& access) {
    auto model = access.readInputRegister(modbusAddress, 148);
    if (model.status != SIStatus::Success && !model.value.isValid()) {
        return Invalid;
    }
    switch (model.value.toInt()) {
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
