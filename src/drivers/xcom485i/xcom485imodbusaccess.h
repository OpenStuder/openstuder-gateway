#pragma once
#include <sipropertyreadresult.h>
#include <sipropertywriteresult.h>
#include <sipropertytype.h>
#include <QVariant>

class XCom485iModbusAccess {
  public:
    virtual ~XCom485iModbusAccess() = default;

    inline SIPropertyReadResult readInputRegister(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type = SIPropertyType::Float) {
        return readInputRegister_(deviceAddress, registerAddress, type);
    }

    inline SIPropertyReadResult readHoldingRegister(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type = SIPropertyType::Float) {
        return readHoldingRegister_(deviceAddress, registerAddress, type);
    }

    inline SIPropertyWriteResult writeHoldingRegister(quint8 deviceAddress, unsigned int registerAddress, const QVariant& value, SIPropertyType type = SIPropertyType::Float) {
        return writeHoldingRegister_(deviceAddress, registerAddress, value, type);
    }

  private:
    virtual SIPropertyReadResult readInputRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) = 0;
    virtual SIPropertyReadResult readHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) = 0;
    virtual SIPropertyWriteResult writeHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, const QVariant& value, SIPropertyType type) = 0;
};
