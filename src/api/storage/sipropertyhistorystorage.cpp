#include "sipropertyhistorystorage.h"

bool SIPropertyHistoryStorage::storePropertyValues(const QMap<SIPropertyID, QVariant>& properties, QDateTime timestamp) {
    return storePropertyValues_(properties, timestamp);
}

QVector<SIPropertyHistoryStorage::TimestampedProperty> SIPropertyHistoryStorage::retrievePropertyValues(SIPropertyID id, QDateTime from, QDateTime to) {
    return retrievePropertyValues_(id, std::move(from), std::move(to));
}
