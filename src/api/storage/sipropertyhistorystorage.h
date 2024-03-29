#pragma once
#include <siglobalpropertyid.h>
#include <sistatus.h>
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

    SIPropertyHistoryStorage() = default;
    SIPropertyHistoryStorage(const SIPropertyHistoryStorage&) = delete;
    SIPropertyHistoryStorage& operator =(const SIPropertyHistoryStorage&) = delete;
    virtual ~SIPropertyHistoryStorage() = default;

    inline bool storePropertyValue(const SIGlobalPropertyID& id, const QVariant& value, const QDateTime& timestamp = QDateTime::currentDateTimeUtc()) {
        return storePropertyValues({{id, value}}, timestamp);
    }

    bool storePropertyValues(const QMap<SIGlobalPropertyID,QVariant>& properties, const QDateTime& timestamp = QDateTime::currentDateTimeUtc());

    QVector<TimestampedProperty> retrievePropertyValues(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to = QDateTime::currentDateTimeUtc(),
                                                        unsigned int limit = std::numeric_limits<unsigned int>::max(), SIStatus* status = nullptr);

    QVector<SIGlobalPropertyID> availableStoredProperties(const QDateTime& from = QDateTime::fromSecsSinceEpoch(0), const QDateTime& to = QDateTime::currentDateTimeUtc(),
                                                          SIStatus* status = nullptr);

  private:
    virtual bool storePropertyValues_(const QMap<SIGlobalPropertyID,QVariant>& properties, const QDateTime& timestamp) = 0;
    virtual QVector<TimestampedProperty> retrievePropertyValues_(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to, unsigned int limit, SIStatus* status) = 0;
    virtual QVector<SIGlobalPropertyID> availableStoredProperties_(const QDateTime& from, const QDateTime& to, SIStatus* status) = 0;
};
