#pragma once
#include "sipropertyid.h"
#include "sipropertytype.h"
#include "siaccesslevel.h"
#include "sipropertyflags.h"
#include <QString>
#include <utility>

struct SIProperty {
    inline SIProperty(): id(0), type(SIPropertyType::Invalid), accessLevel(SIAccessLevel::None), flags(SIPropertyFlag::None) {}
    inline SIProperty(SIPropertyID id, SIPropertyType type, SIAccessLevel accessLevel, SIPropertyFlags flags, QString  description, QString  unit):
        id(id), type(type), accessLevel(accessLevel), flags(flags), description(std::move(description)), unit(std::move(unit)) {}

    SIPropertyID id;
    SIPropertyType type;
    SIAccessLevel accessLevel;
    SIPropertyFlags flags;
    QString description;
    QString unit;
};
