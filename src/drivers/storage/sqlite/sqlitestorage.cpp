#include "sqlitestorage.h"
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlQuery>

Q_DECLARE_LOGGING_CATEGORY(SQLite)

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
    if (!query.exec("CREATE TABLE IF NOT EXISTS property_history (id INTEGER, timestamp INTEGER, value BLOB)") ||
        !query.exec("CREATE UNIQUE INDEX IF NOT EXISTS property_history_index ON property_history (id, timestamp)")) {
        return false;
    }

    return true;
}

bool SQLiteStorage::storePropertyValues_(const QMap<SIPropertyID, QVariant>& properties, QDateTime timestamp) {
    db_.transaction();
    for (const auto& propertyID: properties.keys()) {
        auto query = QSqlQuery(db_);
        query.prepare("INSERT OR REPLACE INTO property_history (id, timestamp, value) VALUES (?, ?, ?)");
        query.addBindValue(propertyID);
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

QVector<SIStorage::TimestampedProperty> SQLiteStorage::retrievePropertyValues_(SIPropertyID id, QDateTime from, QDateTime to) {
    QVector<TimestampedProperty> result;

    auto query = QSqlQuery(db_);
    query.prepare("SELECT timestamp, value FROM property_history WHERE id = ? AND timestamp BETWEEN ? AND ?");
    query.addBindValue(id);
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
