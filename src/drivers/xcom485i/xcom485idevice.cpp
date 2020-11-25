#include "xcom485idevice.h"

XCom485iDevice::XCom485iDevice(const QString& model, quint8 modbusAddress): SIDevice(model, QString::number(modbusAddress)), modbusAddress_(modbusAddress) {

}
const QVector<SIProperty>& XCom485iDevice::properties_() const {
    static QVector<SIProperty> dummy {};
    return dummy;
}

SIPropertyReadResult XCom485iDevice::readProperty_(SIPropertyID id) const {
    return SIPropertyReadResult();
}

SIPropertyWriteResult XCom485iDevice::writeProperty_(SIPropertyID id, const QVariant& value) {
    return SIPropertyWriteResult();
}
