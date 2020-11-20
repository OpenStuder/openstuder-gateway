#pragma once
#include "sipropertyid.h"
#include "sipropertytype.h"
#include "sipropertyflags.h"
#include <QString>

struct SIProperty {
    SIPropertyID id;
    SIPropertyType type;
    SIPropertyFlags  flags;
    QString description;
    QString unit;
};
