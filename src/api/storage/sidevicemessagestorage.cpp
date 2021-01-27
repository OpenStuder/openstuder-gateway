#include "sidevicemessagestorage.h"

bool SIDeviceMessageStorage::storeDeviceMessages(const QVector<SIDeviceMessage>& messages, const QDateTime& timestamp) {
    return storeDeviceMessages_(messages, timestamp);
}

QVector<SIDeviceMessageStorage::TimestampedDeviceMessage> SIDeviceMessageStorage::retrieveDeviceMessages(const QDateTime& from, const QDateTime& to) {
    return retrieveDeviceMessages_(from, to);
}
