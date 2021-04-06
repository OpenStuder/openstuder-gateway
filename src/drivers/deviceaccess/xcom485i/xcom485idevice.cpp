#include "xcom485idevice.h"
#include "xcom485imodbusaccess.h"

using namespace std;

XCom485iDevice::XCom485iDevice(const QString& model, quint8 modbusAddress, XCom485iModbusAccess* modbusAccess, const initializer_list<XCom58iProperty>& properties):
    SIDevice(model, deviceNames.value(modbusAddress, QString::number(modbusAddress))), modbusAddress_(modbusAddress), modbusAccess_(modbusAccess) {
    addProperties(properties);
}

const QMap<quint8,QString> XCom485iDevice::deviceNames {
    {1, "gw"},
    {7, "l1"},
    {8, "l2"},
    {9, "l3"},
    {10, "xts"},
    {11, "xt1"},
    {12, "xt2"},
    {13, "xt3"},
    {14, "xt4"},
    {15, "xt5"},
    {16, "xt6"},
    {17, "xt7"},
    {18, "xt8"},
    {19, "xt9"},
    {20, "vts"},
    {21, "vt1"},
    {22, "vt2"},
    {23, "vt3"},
    {24, "vt4"},
    {25, "vt5"},
    {26, "vt6"},
    {27, "vt7"},
    {28, "vt8"},
    {29, "vt9"},
    {30, "vt10"},
    {31, "vt11"},
    {32, "vt12"},
    {33, "vt13"},
    {34, "vt14"},
    {35, "vt15"},
    {40, "vss"},
    {41, "vs1"},
    {42, "vs2"},
    {43, "vs3"},
    {44, "vs4"},
    {45, "vs5"},
    {46, "vs6"},
    {47, "vs7"},
    {48, "vs8"},
    {49, "vs9"},
    {50, "vs10"},
    {51, "vs11"},
    {52, "vs12"},
    {53, "vs13"},
    {54, "vs14"},
    {55, "vs15"},
    {61, "bat"},
};

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
    result.setID(id);
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

    SIPropertyWriteResult result = flags.testFlag(SIPropertyWriteFlag::Permanent) ?
        modbusAccess_->writeHoldingRegister(modbusAddress_, *registerAddress, value, property->type()) :
        modbusAccess_->writeHoldingRegister(modbusAddress_, *registerAddress + 6000, value, property->type());
    result.setID(id);
    return result;
}
