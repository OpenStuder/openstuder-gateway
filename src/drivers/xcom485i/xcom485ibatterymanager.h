#pragma once
#include "xcom485idevice.h"

class XCom485iBatteryManager: public XCom485iDevice {
  public:
    enum Model {
        Invalid,
        BSP,
        XComCAN_BMS
    };

    explicit XCom485iBatteryManager(Model model);

    inline Model xComModel() const {
        return model_;
    }

    static Model model(XCom485iModbusAccess& access);

    static QString toString(Model model);

  private:
    Model model_;
};
