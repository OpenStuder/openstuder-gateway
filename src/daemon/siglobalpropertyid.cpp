#include "siglobalpropertyid.h"
#include <QStringList>
#include <utility>

using namespace std;

SIGlobalPropertyID::SIGlobalPropertyID(const QString& uniqueID) {
    auto split = uniqueID.split("/");

    switch (split.count()) {
        case 2:
            accessID_ = split[0];
            propertyID_ = split[1].toLongLong();
            break;

        case 3:
            accessID_ = split[0];
            deviceID_ = split[1];
            propertyID_ = split[2].toLongLong();
            break;
    }
}

SIGlobalPropertyID::SIGlobalPropertyID(QString accessID, QString deviceID, SIPropertyID propertyID): accessID_(move(accessID)), deviceID_(move(deviceID)), propertyID_(propertyID) {}

bool SIGlobalPropertyID::isValid() const {
    return propertyID_ != 0;
}
