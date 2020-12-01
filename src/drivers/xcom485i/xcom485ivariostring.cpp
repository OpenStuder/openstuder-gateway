#include "xcom485ivariostring.h"

XCom485iVarioString::XCom485iVarioString(XCom485iVarioString::Model model, quint8 modbusAddress): XCom485iDevice(toString(model), modbusAddress, {}) {}

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
        case Invalid:
            return "Invalid VarioString model";

        case Multicast:
            return "VarioString multicast";

        case VS120:
            return "VarioString VS120";

        case VS70:
            return "VarioString VS70";
    }
}
