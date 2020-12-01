#pragma once
#include <sidevice.h>
#include "xcom485imodbusaccess.h"
#include "xcom485iproperty.h"

class XCom485iDevice: public SIDevice {
  public:
    explicit XCom485iDevice(const QString& model, quint8 modbusAddress, const std::initializer_list<XCom58iProperty>& properties);

    inline quint8 modbusAddress() const {
        return modbusAddress_;
    }

  protected:
    void addProperties(std::initializer_list<XCom58iProperty> properties);

  private:
    const QVector<SIProperty>& properties_() const override;
    SIPropertyReadResult readProperty_(SIPropertyID id) const override;
    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) override;

    quint8 modbusAddress_;
    QVector<SIProperty> deviceProperties_;
    QMap<SIPropertyID,int> modbusAddresses_;
};
