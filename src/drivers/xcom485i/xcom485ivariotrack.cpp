#include "xcom485ivariotrack.h"

XCom485iVarioTrack::XCom485iVarioTrack(Model model, quint8 modbusAddress): XCom485iDevice(toString(model), modbusAddress) {}

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
