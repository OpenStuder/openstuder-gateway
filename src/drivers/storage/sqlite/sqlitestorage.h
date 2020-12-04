#pragma once
#include <sistorage.h>
#include <QSqlDatabase>

class SQLiteStorage: public SIStorage {
  public:
    bool open(const QString& filename);

  private:
    QSqlDatabase db_;

    bool storePropertyValues_(const QMap<SIPropertyID, QVariant>& properties, QDateTime timestamp) override;
    QVector<TimestampedProperty> retrievePropertyValues_(SIPropertyID id, QDateTime from, QDateTime to) override;
};


