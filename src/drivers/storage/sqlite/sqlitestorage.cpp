#include "sqlitestorage.h"
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlQuery>

Q_DECLARE_LOGGING_CATEGORY(SQLite)

SQLiteStorage::~SQLiteStorage() {
    db_.close();
}

bool SQLiteStorage::open(const QString& filename) {
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCCritical(SQLite,) << "SQLITE driver not available";
        return false;
    }

    db_ = QSqlDatabase::addDatabase("QSQLITE");
    if (!db_.isValid()) {
        qCCritical(SQLite,) << "Unable to create SQLITE database";
        return false;
    }
    db_.setDatabaseName(filename);
    if (!db_.open()) {
        qCCritical(SQLite,) << "Unable to open SQLITE database" << filename;
        return false;
    }

    auto query = QSqlQuery(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS property_history (id STRING, timestamp INTEGER, value BLOB)") ||
        !query.exec("CREATE UNIQUE INDEX IF NOT EXISTS property_history_index ON property_history (id, timestamp)") ||
        !query.exec("CREATE TABLE IF NOT EXISTS device_message (timestamp INTEGER, access_id STRING, device_id STRING, message_id INTEGER, message STRING)")) {
        return false;
    }

    return true;
}

bool SQLiteStorage::storePropertyValues_(const QMap<SIGlobalPropertyID, QVariant>& properties, const QDateTime& timestamp) {
    db_.transaction();
    for (const auto& propertyID: properties.keys()) {
        auto query = QSqlQuery(db_);
        query.prepare("INSERT OR REPLACE INTO property_history (id, timestamp, value) VALUES (?, ?, ?)");
        query.addBindValue(propertyID.toString());
        query.addBindValue(timestamp);
        query.addBindValue(properties[propertyID]);
        if (!query.exec()) {
            db_.rollback();
            return false;
        }
    }
    db_.commit();
    return true;
}

QVector<SIStorage::TimestampedProperty> SQLiteStorage::retrievePropertyValues_(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to) {
    QVector<TimestampedProperty> result;

    auto query = QSqlQuery(db_);
    query.prepare("SELECT timestamp, value FROM property_history WHERE id = ? AND timestamp BETWEEN ? AND ?");
    query.addBindValue(id.toString());
    query.addBindValue(from);
    query.addBindValue(to);

    if (!query.exec()) {
        return result;
    }

    while (query.next()) {
        result.append({query.value(0).toDateTime(), query.value(1)});
    }

    return result;
}

bool SQLiteStorage::storeDeviceMessages_(const QVector<SIDeviceMessage>& messages, const QDateTime& timestamp) {
    db_.transaction();
    for (const auto& message: messages) {
        auto query = QSqlQuery(db_);
        query.prepare("INSERT INTO device_message (timestamp, access_id, device_id, message_id, message) VALUES (?, ?, ?, ?, ?)");
        query.addBindValue(timestamp.toSecsSinceEpoch());
        query.addBindValue(message.accessID);
        query.addBindValue(message.deviceID);
        query.addBindValue(message.messageID);
        query.addBindValue(message.message);
        if (!query.exec()) {
            db_.rollback();
            return false;
        }
    }
    db_.commit();
    return true;
}

QVector<SIStorage::TimestampedDeviceMessage> SQLiteStorage::retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit) {
    QVector<TimestampedDeviceMessage> result;

    auto query = QSqlQuery(db_);
    query.prepare("SELECT timestamp, access_id, device_id, message_id, message FROM device_message WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC limit ?");
    query.addBindValue(from.toSecsSinceEpoch());
    query.addBindValue(to.toSecsSinceEpoch());
    query.addBindValue(limit);

    if (!query.exec()) {
        return result;
    }

    while (query.next()) {
        result.append({
            QDateTime::fromSecsSinceEpoch(query.value(0).toULongLong()), {
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toUInt(),
                query.value(4).toString()
            }
        });
    }

    return result;
}
