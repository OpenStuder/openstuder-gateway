#pragma once
#include <sipropertyid.h>
#include <QDateTime>
#include <QVariant>
#include <QVector>
#include <QMap>

class SIPropertyHistoryStorage {
  public:
    struct TimestampedProperty {
        QDateTime timestamp;
        QVariant value;
    };

    virtual ~SIPropertyHistoryStorage() = default;

    inline bool storePropertyValue(SIPropertyID id, const QVariant& value, QDateTime timestamp = QDateTime::currentDateTime()) {
        return storePropertyValues({{id, value}}, timestamp);
    }

    bool storePropertyValues(const QMap<SIPropertyID,QVariant>& properties, QDateTime timestamp = QDateTime::currentDateTime());

    QVector<TimestampedProperty> retrievePropertyValues(SIPropertyID id, QDateTime from, QDateTime to = QDateTime::currentDateTime());

  private:
    virtual bool storePropertyValues_(const QMap<SIPropertyID,QVariant>& properties, QDateTime timestamp) = 0;
    virtual QVector<TimestampedProperty> retrievePropertyValues_(SIPropertyID id, QDateTime from, QDateTime to) = 0;
};
