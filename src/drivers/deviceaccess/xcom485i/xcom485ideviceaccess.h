#pragma once
#include <sideviceaccess.h>
#include "xcom485imodbusaccess.h"
#include <sipropertywriteresult.h>
#include <QSerialPort>
#include <QModbusRtuSerialMaster>

class XCom485iDeviceAccess: public SIDeviceAccess, private XCom485iModbusAccess {
  public:
    explicit XCom485iDeviceAccess(const QString& id);

    bool open(const QString& port, unsigned int baudRate, quint8 deviceOffset);

    inline bool forceSlowEnumeration() const {
        return forceSlowEnumeration_;
    }

    inline void setForceSlowEnumeration(bool forceSlowEnumeration) {
        forceSlowEnumeration_ = forceSlowEnumeration;
    }

  private:
    void retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const override;
    bool enumerateDevices_(QVector<SIDevice*>& devices) override;

    void completeJsonDescription_(QJsonObject& object, SIDescriptionFlags flags) const override;

    SIPropertyReadResult readInputRegister_(quint8 deviceAddress, unsigned int propertyModbusAddress, SIPropertyType type) override;
    SIPropertyReadResult readHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, SIPropertyType type) override;
    SIPropertyWriteResult writeHoldingRegister_(quint8 deviceAddress, unsigned int registerAddress, const QVariant& value, SIPropertyType type) override;

    mutable QModbusRtuSerialMaster modbus_;
    quint8 deviceOffset_;
    bool forceSlowEnumeration_ = false;
};
