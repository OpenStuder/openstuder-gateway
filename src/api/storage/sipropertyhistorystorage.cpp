#include "sipropertyhistorystorage.h"

bool SIPropertyHistoryStorage::storePropertyValues(const QMap<SIGlobalPropertyID, QVariant>& properties, const QDateTime& timestamp) {
    return storePropertyValues_(properties, timestamp);
}

QVector<SIPropertyHistoryStorage::TimestampedProperty> SIPropertyHistoryStorage::retrievePropertyValues(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to) {
    return retrievePropertyValues_(id, from, to);
}
