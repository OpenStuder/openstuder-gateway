#include "xcom485ivariotrack.h"

XCom485iVarioTrack::XCom485iVarioTrack(Model model, quint8 modbusAddress): XCom485iDevice(toString(model), modbusAddress, {
    {0, 11000, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery voltage", "Vdc"},
    {2, 11001, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery current", "Adc"},

    {10, 10005, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Floating voltage", "Vdc"},
    {18, 1009, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Absorption voltage", "Vdc"},

    {4, 10002, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Battery charge current", "Adc"},
    {668, 10334, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Battery undervoltage", "Vdc"}
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
        case Invalid:
            return "Invalid VarioTrack model";

        case Multicast:
            return "VarioTrack multicast";

        case VT80:
            return "VarioTrack VT-80";

        case VT65:
            return "VarioTrack VT-65";
    }
}
