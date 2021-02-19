#include "sidevicemessage.h"
#include <memory>
#include <utility>

using namespace std;

static const QDateTime emptyTimestamp_ {};
static const QString emptyString_ {};

struct SIDeviceMessage::Private_ {
    Private_(QDateTime timestamp, QString accessId, QString deviceId, quint64 messageId, QString message):
        timestamp(move(timestamp)), accessID(move(accessId)), deviceID(move(deviceId)), messageID(messageId), message(move(message)) {}

    QDateTime timestamp;
    QString accessID;
    QString deviceID;
    quint64 messageID;
    QString message;
};

SIDeviceMessage::SIDeviceMessage() = default;

SIDeviceMessage::SIDeviceMessage(QString accessID, QString deviceID, quint64 messageID, QString message, QDateTime timestamp)
    : private_(make_shared<Private_>(move(timestamp), move(accessID), move(deviceID), messageID, move(message))) {}

bool SIDeviceMessage::isValid() const {
    return private_ != nullptr;
}

const QDateTime& SIDeviceMessage::timestamp() const {
    if (private_) {
        return private_->timestamp;
    } else {
        return emptyTimestamp_;
    }
}

const QString& SIDeviceMessage::accessID() const {
    if (private_) {
        return private_->accessID;
    } else {
        return emptyString_;
    }
}

void SIDeviceMessage::setAccessID(const QString& accessID) {
    if (private_) {
        detach_();
        private_->accessID = accessID;
    }
}

const QString& SIDeviceMessage::deviceID() const {
    if (private_) {
        return private_->deviceID;
    } else {
        return emptyString_;
    }
}

quint64 SIDeviceMessage::messageID() const {
    if (private_) {
        return private_->messageID;
    } else {
        return 0;
    }
}

const QString& SIDeviceMessage::message() const {
    if (private_) {
        return private_->message;
    } else {
        return emptyString_;
    }
}

void SIDeviceMessage::detach_() {
    if (private_.use_count() > 1) {
        private_ = make_shared<Private_>(*private_);
    }
}
