#pragma once
#include <siproperty.h>

struct XCom58iProperty: public SIProperty {
    inline XCom58iProperty(int modbusAddress, SIPropertyID id, SIPropertyType type, SIAccessLevel accessLevel, SIPropertyFlags flags, const QString& description, const QString& unit):
        SIProperty(id, type, accessLevel, flags, description, unit), modbusAddress(modbusAddress) {}

    int modbusAddress;
};
