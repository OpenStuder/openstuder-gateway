#pragma once
#include <sidevicemessage.h>
#include <QDateTime>
#include <QVector>

class SIDeviceMessageStorage {
  public:
    struct TimestampedDeviceMessage {
        QDateTime timestamp;
        SIDeviceMessage message;
    };

    virtual ~SIDeviceMessageStorage() = default;

    inline bool storeDeviceMessage(const SIDeviceMessage& message, const QDateTime& timestamp = QDateTime::currentDateTime()) {
        return storeDeviceMessages({message}, timestamp);
    }

    bool storeDeviceMessages(const QVector<SIDeviceMessage>& messages, const QDateTime& timestamp = QDateTime::currentDateTime());

    QVector<TimestampedDeviceMessage> retrieveDeviceMessages(const QDateTime& from, const QDateTime& to = QDateTime::currentDateTime(), unsigned int limit = std::numeric_limits<unsigned int>::max());

  private:
    virtual bool storeDeviceMessages_(const QVector<SIDeviceMessage>& messages, const QDateTime& timestamp) = 0;
    virtual QVector<TimestampedDeviceMessage> retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit) = 0;
};
