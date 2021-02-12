#pragma once
#include <sistorage.h>
#include <QSqlDatabase>

class SQLiteStorage: public SIStorage {
  public:
    ~SQLiteStorage();

    bool open(const QString& filename);

  private:
    QSqlDatabase db_;

    bool storePropertyValues_(const QMap<SIGlobalPropertyID, QVariant>& properties, const QDateTime& timestamp) override;
    QVector<TimestampedProperty> retrievePropertyValues_(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to, unsigned int limit) override;

    bool storeDeviceMessages_(const QVector<SIDeviceMessage>& messages) override;
    QVector<SIDeviceMessage> retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit) override;
};
