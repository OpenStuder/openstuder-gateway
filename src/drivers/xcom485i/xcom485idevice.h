#pragma once
#include <sidevice.h>
#include "xcom485imodbusaccess.h"

class XCom485iDevice: public SIDevice {
  public:
    explicit XCom485iDevice(const QString& model, quint8 modbusAddress);

    inline quint8 modbusAddress() const {
        return modbusAddress_;
    }

  private:
    const QVector<SIProperty>& properties_() const override;
    SIPropertyReadResult readProperty_(SIPropertyID id) const override;
    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value) override;

    quint8 modbusAddress_;
};
