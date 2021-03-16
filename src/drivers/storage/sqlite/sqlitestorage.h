#pragma once
#include <sistorage.h>
#include <QSqlDatabase>

class SQLiteStorage: public QObject, public SIStorage {
    Q_OBJECT

  public:
    ~SQLiteStorage() override;

    bool open(const QString& filename, int cleanupInterval, int maximalStorageDays);

  private:
    bool storePropertyValues_(const QMap<SIGlobalPropertyID, QVariant>& properties, const QDateTime& timestamp) override;
    QVector<TimestampedProperty> retrievePropertyValues_(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to, unsigned int limit, SIStatus* status) override;
    QVector<SIGlobalPropertyID> availableStoredProperties_(const QDateTime& from, const QDateTime& to, SIStatus* status) override;

    bool storeDeviceMessages_(const QVector<SIDeviceMessage>& messages) override;
    QVector<SIDeviceMessage> retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit, SIStatus* status) override;

    void timerEvent(QTimerEvent* event) override;

    QSqlDatabase db_;
    int maximalStorageDays_;
};
