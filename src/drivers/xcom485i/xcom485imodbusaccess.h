#pragma once
#include <sipropertyreadresult.h>
#include <sipropertytype.h>
#include <QVariant>

class XCom485iModbusAccess {
  public:
    virtual ~XCom485iModbusAccess() = default;

    virtual SIPropertyReadResult readInputRegister(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type = SIPropertyType::Float) = 0;
};
