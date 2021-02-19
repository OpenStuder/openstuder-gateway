#include "siglobalpropertyid.h"
#include <QStringList>
#include <utility>

using namespace std;

static const QString emptyString_ {};

struct SIGlobalPropertyID::Private_ {
    Private_(QString accessId, QString deviceId, SIPropertyID propertyId): accessID(std::move(accessId)), deviceID(std::move(deviceId)), propertyID(propertyId) {}

    QString accessID;
    QString deviceID;
    SIPropertyID propertyID = 0;
};

SIGlobalPropertyID::SIGlobalPropertyID() = default;

SIGlobalPropertyID::SIGlobalPropertyID(const char* uniqueID): SIGlobalPropertyID(QString(uniqueID)) {}

SIGlobalPropertyID::SIGlobalPropertyID(const QByteArray& uniqueID): SIGlobalPropertyID(QString(uniqueID)) {}

SIGlobalPropertyID::SIGlobalPropertyID(const QString& uniqueID) {
    auto split = uniqueID.split(".");
    if (split.count() == 3) {
        bool validPropertyID = true;
        auto propertyID = split[2].toLongLong(&validPropertyID);
        if (validPropertyID) {
            auto accessID = split[0];
            auto deviceID = split[1];
            private_ = make_shared<Private_>(accessID, deviceID, propertyID);
        }
    }
}

SIGlobalPropertyID::SIGlobalPropertyID(QString accessID, QString deviceID, SIPropertyID propertyID): private_(make_shared<Private_>(accessID, deviceID, propertyID)) {}

bool SIGlobalPropertyID::isValid() const {
    return private_ != nullptr;
}

const QString& SIGlobalPropertyID::accessID() const {
    return private_ ? private_->accessID : emptyString_;
}

const QString& SIGlobalPropertyID::deviceID() const {
    return private_ ? private_->deviceID : emptyString_;
}

SIPropertyID SIGlobalPropertyID::propertyID() const {
    return private_ ? private_->propertyID : 0;
}

QString SIGlobalPropertyID::toString() const {
    return private_ ? QString("%1.%2.%3").arg(private_->accessID).arg(private_->deviceID).arg(private_->propertyID) : emptyString_;
}

bool SIGlobalPropertyID::isWildcard() const {
    return private_ && (private_->accessID == "*" || private_->deviceID == "*");
}

bool SIGlobalPropertyID::matches(const SIGlobalPropertyID& id) const {
    return private_ && id.isValid() && matches(id.accessID(), id.deviceID(), id.propertyID());
}

bool SIGlobalPropertyID::matches(const QString& accessID, const QString& deviceID, SIPropertyID propertyID) const {
    return private_ &&
           isWildcard() &&
           private_->propertyID == propertyID &&
           (private_->deviceID == "*" || private_->deviceID == deviceID) &&
           (private_->accessID == "*" || private_->accessID == accessID);
}

bool SIGlobalPropertyID::operator ==(const SIGlobalPropertyID& other) const {
    return (private_ == nullptr && !other.isValid()) ||
           (private_ && other.isValid() &&
            private_->accessID == other.accessID() &&
            private_->deviceID == other.deviceID() &&
            private_->propertyID == other.propertyID());
}

bool SIGlobalPropertyID::operator <(const SIGlobalPropertyID& other) const {
    return private_ && other.isValid() &&
           (private_->accessID != other.accessID() ? private_->accessID < other.accessID() :
            private_->deviceID != other.deviceID() ? private_->deviceID < other.deviceID() :
            private_->propertyID < other.propertyID());
}
