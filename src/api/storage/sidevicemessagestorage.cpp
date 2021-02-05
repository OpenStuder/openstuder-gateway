#include "sidevicemessagestorage.h"

bool SIDeviceMessageStorage::storeDeviceMessages(const QVector<SIDeviceMessage>& messages, const QDateTime& timestamp) {
    return storeDeviceMessages_(messages, timestamp);
}

QVector<SIDeviceMessageStorage::TimestampedDeviceMessage> SIDeviceMessageStorage::retrieveDeviceMessages(const QDateTime& from, const QDateTime& to, unsigned int limit) {
    return retrieveDeviceMessages_(from, to, limit);
}
