#include "xcom485ivariostring.h"
#include "xcom485imodbusaccess.h"

XCom485iVarioString::XCom485iVarioString(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), modbusAddress, modbusAccess, {
    {0, 15000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage", "Vdc"},
    {2, 15001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current", "Adc"},

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
