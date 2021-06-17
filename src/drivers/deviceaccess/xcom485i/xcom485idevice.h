#pragma once
#include <sidevice.h>
#include "xcom485iproperty.h"
#include <QMap>

class XCom485iModbusAccess;

class XCom485iDevice: public SIDevice {
  public:
    explicit XCom485iDevice(const QString& model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess, const std::initializer_list<XCom58iProperty>& properties, bool isVirtual = false);

    inline quint8 modbusAddress() const {
        return modbusAddress_;
    }

    XCom485iModbusAccess* modbusAccess() const {
        return modbusAccess_;
    }

    static const QMap<quint8,QString> deviceNames;

  protected:
    void addProperties(std::initializer_list<XCom58iProperty> properties);

  private:
    quint8 modbusAddress_;
    QVector<SIProperty> deviceProperties_;
    QMap<SIPropertyID,int> modbusRegisterAddresses_;
    XCom485iModbusAccess* modbusAccess_;

    const QVector<SIProperty>& properties_() const override;

    SIPropertyReadResult readProperty_(SIPropertyID id) const override;
    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) override;
};
