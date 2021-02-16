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
    {16, 3008, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Low Voltage Disconnect", "V"},
    {20, 3010, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery cycle phase", {
        {0, "Invalid value"},
        {1, "Bulk"},
        {2, "Absorption"},
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
    {36, 3018, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input limit reached", {
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

    {274, 3137, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Input active power", "kW"},
    {272, 3136, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output active power", "kW"},

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
