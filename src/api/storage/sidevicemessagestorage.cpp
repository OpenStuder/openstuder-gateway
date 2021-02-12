#include "sidevicemessagestorage.h"

bool SIDeviceMessageStorage::storeDeviceMessages(const QVector<SIDeviceMessage>& messages) {
    return storeDeviceMessages_(messages);
}

QVector<SIDeviceMessage> SIDeviceMessageStorage::retrieveDeviceMessages(const QDateTime& from, const QDateTime& to, unsigned int limit) {
    return retrieveDeviceMessages_(from, to, limit);
}
