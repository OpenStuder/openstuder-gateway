#pragma once
#include <sideviceaccess.h>
#include "xcom485imodbusaccess.h"
#include <QSerialPort>
#include <QModbusRtuSerialMaster>

class XCom485iDeviceAccess: public SIDeviceAccess, private XCom485iModbusAccess {
  public:
    explicit XCom485iDeviceAccess(const QString& id);

    bool open(const QString& port, unsigned int baudRate, unsigned int deviceOffset);

  private:
    void retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const override;
    bool enumerateDevices_(QVector<SIDevice*>& devices) override;
    void completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const override;

    SIPropertyReadResult readInputRegister(quint8 deviceAddress, unsigned int propertyModbusAddress, SIPropertyType type) override;

    mutable QModbusRtuSerialMaster modbus_;
    unsigned int deviceOffset_;
};
