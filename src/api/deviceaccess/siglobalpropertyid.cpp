#include "siglobalpropertyid.h"
#include <QStringList>
#include <utility>

using namespace std;

SIGlobalPropertyID::SIGlobalPropertyID(const QString& uniqueID) {
    auto split = uniqueID.split(".");
    if (split.count() == 3) {
        bool validPropertyID = true;
        propertyID_ = split[2].toLongLong(&validPropertyID);
        if (validPropertyID) {
            accessID_ = split[0];
            deviceID_ = split[1];
        }
    }
}

SIGlobalPropertyID::SIGlobalPropertyID(QString accessID, QString deviceID, SIPropertyID propertyID): accessID_(move(accessID)), deviceID_(move(deviceID)), propertyID_(propertyID) {}

bool SIGlobalPropertyID::isValid() const {
    return !deviceID_.isEmpty();
}

QString SIGlobalPropertyID::toString() const {
    if (isValid()) {
        return QString("%1.%2.%3").arg(accessID_).arg(deviceID_).arg(propertyID_);
    } else {
        return {};
    }
}

bool SIGlobalPropertyID::operator ==(const SIGlobalPropertyID& other) const {
    return accessID_ == other.accessID_ &&
    deviceID_ == other.deviceID_ &&
    propertyID_ == other.propertyID_;
}

bool SIGlobalPropertyID::operator <(const SIGlobalPropertyID& other) const {
    return accessID_ != other.accessID_ ? accessID_ < other.accessID_ :
           deviceID_ != other.deviceID_ ? deviceID_ < other.deviceID_ :
           propertyID_ < other.propertyID_;
}
