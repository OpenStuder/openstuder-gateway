#pragma once
#include <sidevicemessage.h>
#include <sistatus.h>
#include <QDateTime>
#include <QVector>

class SIDeviceMessageStorage {
  public:
    SIDeviceMessageStorage() = default;
    SIDeviceMessageStorage(const SIDeviceMessageStorage&) = delete;
    SIDeviceMessageStorage& operator =(const SIDeviceMessageStorage&) = delete;
    virtual ~SIDeviceMessageStorage() = default;

    inline bool storeDeviceMessage(const SIDeviceMessage& message) {
        return storeDeviceMessages({message});
    }

    bool storeDeviceMessages(const QVector<SIDeviceMessage>& messages);

    QVector<SIDeviceMessage> retrieveDeviceMessages(const QDateTime& from, const QDateTime& to , unsigned int limit = std::numeric_limits<unsigned int>::max(), SIStatus* status = nullptr);

  private:
    virtual bool storeDeviceMessages_(const QVector<SIDeviceMessage>& messages) = 0;
    virtual QVector<SIDeviceMessage> retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit, SIStatus* status) = 0;
};
