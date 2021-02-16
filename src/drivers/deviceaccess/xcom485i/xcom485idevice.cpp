#include "xcom485idevice.h"
#include "xcom485imodbusaccess.h"

using namespace std;

XCom485iDevice::XCom485iDevice(const QString& model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess, const initializer_list<XCom58iProperty>& properties):
    SIDevice(model, QString::number(modbusAddress)), modbusAddress_(modbusAddress), modbusAccess_(modbusAccess) {
    addProperties(properties);
}

void XCom485iDevice::addProperties(std::initializer_list<XCom58iProperty> properties) {
    // Copy properties and create ID to modbus address map.
    deviceProperties_.reserve(properties.size());
    for (const auto& property: properties) {
        deviceProperties_.append(property);
        modbusRegisterAddresses_[property.id()] = property.modbusAddress;
    }
}

const QVector<SIProperty>& XCom485iDevice::properties_() const {
    return deviceProperties_;
}

SIPropertyReadResult XCom485iDevice::readProperty_(SIPropertyID id) const {
    // Check if property exists.
    auto property = std::find_if(deviceProperties_.cbegin(), deviceProperties_.cend(), [&id](const SIProperty& p) { return p.id() == id; });
    if (property == deviceProperties_.cend()) return {id, SIStatus::NoProperty, {}};
    if (!property->isFlagSet(SIPropertyFlag::Readable)) return {id, SIStatus::Error, {}};

    auto registerAddress = modbusRegisterAddresses_.find(id);
    if (registerAddress == modbusRegisterAddresses_.cend()) return {id, SIStatus::NoProperty, {}};

    SIPropertyReadResult result;
    if (property->isFlagSet(SIPropertyFlag::Writeable)) {
        result = modbusAccess_->readHoldingRegister(modbusAddress_, *registerAddress, property->type());
    } else {
        result = modbusAccess_->readInputRegister(modbusAddress_, *registerAddress, property->type());
    }
    result.id = id;
    return result;
}

SIPropertyWriteResult XCom485iDevice::writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) {
    // Check if property exists.
    auto property = std::find_if(deviceProperties_.cbegin(), deviceProperties_.cend(), [&id](const SIProperty& p) { return p.id() == id; });
    if (property == deviceProperties_.cend()) return {id, SIStatus::NoProperty};
    if (!property->isFlagSet(SIPropertyFlag::Writeable)) {
        return {id, SIStatus::Error};
    }

    auto registerAddress = modbusRegisterAddresses_.find(id);
    if (registerAddress == modbusRegisterAddresses_.cend()) {
        return {id, SIStatus::NoProperty};
    }

    if (property->type() != SIPropertyType::Signal && !value.isValid()) {
        return {id, SIStatus::InvalidValue};
    }

    SIPropertyWriteResult result = {};
    if (flags.testFlag(SIPropertyWriteFlag::Permanent)) {
        result = modbusAccess_->writeHoldingRegister(modbusAddress_, *registerAddress, value, property->type());
    } else {
        result = modbusAccess_->writeHoldingRegister(modbusAddress_ + 6000, *registerAddress, value, property->type());
    }
    result.id = id;
    return result;
}
