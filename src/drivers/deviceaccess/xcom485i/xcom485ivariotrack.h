#pragma once
#include "xcom485idevice.h"

class XCom485iVarioTrack: public XCom485iDevice {
  public:
    enum Model {
        Invalid,
        Multicast,
        VT80,
        VT65
    };

    explicit XCom485iVarioTrack(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess);

    static Model model(quint8 modbusAddress, XCom485iModbusAccess& access);

    static QString toString(Model model);
};


