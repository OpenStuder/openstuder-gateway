#include "sipropertyhistorystorage.h"

bool SIPropertyHistoryStorage::storePropertyValues(const QMap<SIGlobalPropertyID, QVariant>& properties, const QDateTime& timestamp) {
    return storePropertyValues_(properties, timestamp);
}

QVector<SIPropertyHistoryStorage::TimestampedProperty> SIPropertyHistoryStorage::retrievePropertyValues(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to, unsigned int limit) {
    return retrievePropertyValues_(id, from, to, limit);
}
