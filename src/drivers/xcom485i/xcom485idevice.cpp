#include "xcom485idevice.h"

using namespace std;

XCom485iDevice::XCom485iDevice(const QString& model, quint8 modbusAddress, initializer_list<XCom58iProperty> properties):
    SIDevice(model, QString::number(modbusAddress)), modbusAddress_(modbusAddress) {

    // Copy properties and create ID to modbus address map.
    deviceProperties_.reserve(properties.size());
    for (const auto& property: properties) {
        deviceProperties_.append(property);
        modbusAddresses_[property.id] = property.modbusAddress;
    }
}

const QVector<SIProperty>& XCom485iDevice::properties_() const {
    return deviceProperties_;
}

SIPropertyReadResult XCom485iDevice::readProperty_(SIPropertyID id) const {
    return SIPropertyReadResult();
}

SIPropertyWriteResult XCom485iDevice::writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) {
    return SIPropertyWriteResult();
}
