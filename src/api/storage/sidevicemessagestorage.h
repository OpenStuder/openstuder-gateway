#pragma once
#include <sidevicemessage.h>
#include <QDateTime>
#include <QVector>

class SIDeviceMessageStorage {
  public:
    virtual ~SIDeviceMessageStorage() = default;

    inline bool storeDeviceMessage(const SIDeviceMessage& message) {
        return storeDeviceMessages({message});
    }

    bool storeDeviceMessages(const QVector<SIDeviceMessage>& messages);

    QVector<SIDeviceMessage> retrieveDeviceMessages(const QDateTime& from, const QDateTime& to , unsigned int limit = std::numeric_limits<unsigned int>::max());

  private:
    virtual bool storeDeviceMessages_(const QVector<SIDeviceMessage>& messages) = 0;
    virtual QVector<SIDeviceMessage> retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit) = 0;
};
