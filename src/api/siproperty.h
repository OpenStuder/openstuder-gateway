#pragma once
#include "sipropertyid.h"
#include "sipropertytype.h"
#include "sipropertyflags.h"
#include <QString>
#include <utility>

struct SIProperty {
    inline SIProperty(SIPropertyID id, SIPropertyType type, SIPropertyFlags flags, QString  description, QString  unit):
        id(id), type(type), flags(flags), description(std::move(description)), unit(std::move(unit)) {}

    SIPropertyID id;
    SIPropertyType type;
    SIPropertyFlags flags;
    QString description;
    QString unit;
};
