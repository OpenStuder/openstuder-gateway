#pragma once
#include "xcom485idevice.h"

class XCom485iVarioString: public XCom485iDevice {
  public:
    enum Model {
        Invalid,
        Multicast,
        VS120,
        VS70
    };

    explicit XCom485iVarioString(Model model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess);

    static Model model(quint8 modbusAddress, XCom485iModbusAccess& access);

    static QString toString(Model model);
};

