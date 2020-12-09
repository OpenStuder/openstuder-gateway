#include "xcom485ixtender.h"
#include "xcom485imodbusaccess.h"

XCom485iXtender::XCom485iXtender(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess): XCom485iDevice(toString(model), modbusAddress, modbusAccess, {
    {0, 3000, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery voltage", "Vdc"},
    {2, 3001, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery temperature", "°C"},
    {10, 3005, SIPropertyType::Float, SIPropertyFlag::Readable, "Battery charge current", "Adc"},
    {46, 3023, SIPropertyType::Float, SIPropertyFlag::Readable, "Output power", "kVA"},

    {14, 1107, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum current of AC source (Input limit)", "Aac"},
    {76, 1138, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"},
    {48, 1124, SIPropertyType::Bool, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter allowed", ""},

    {50, 1125, SIPropertyType::Bool, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Charger allowed", ""},
    {78, 1139, SIPropertyType::Float, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Temperature compensation", "mV/°C/cell"},
    {212, 1206, SIPropertyType::TimeOfDay, SIPropertyFlag::Readable | SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "Start hour (AUX 1)", "hh:mm"},
    {630, 1415, SIPropertyType::Signal, SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "ON of the Xtenders", ""},
    {598, 1399, SIPropertyType::Signal, SIPropertyFlag::Writeable | SIPropertyFlag::Expert, "OFF of the Xtenders", ""}
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
