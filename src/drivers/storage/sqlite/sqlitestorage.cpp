#include "sqlitestorage.h"
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDir>
#include <QSqlError>
#include <algorithm>

Q_DECLARE_LOGGING_CATEGORY(SQLite)

SQLiteStorage::~SQLiteStorage() {
    // Close database.
    db_.close();
}

bool SQLiteStorage::open(const QString& filename, int cleanupInterval, int maximalStorageDays) {
    maximalStorageDays_ = maximalStorageDays;

    // Check if SQLite driver is available.
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCCritical(SQLite,) << "Qt SQLITE driver not available";
        return false;
    }

    // Ensure that the folder exists and check if the database file already exists.
    QFileInfo fileInfo(filename);
    if (!fileInfo.exists()) {
        auto folder = fileInfo.absoluteDir();
        if (!folder.exists()) {
            qCWarning(SQLite,) << "Folder" << folder.absolutePath() << "does not exist, trying to create it";
            if (!folder.mkpath(".")) {
                qCCritical(SQLite,) << "Failed to create folder" << folder.absolutePath();
                return false;
            }
        }
        qCInfo(SQLite,) << "Creating new SQLite database file" << filename;
    } else {
        qCInfo(SQLite,) << "Opening existing SQLite database file" << filename;
    }

    // Create database.
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    if (!db_.isValid()) {
        qCCritical(SQLite,) << "Unable to create SQLITE database";
        return false;
    }

    // Open database file.
    db_.setDatabaseName(filename);
    if (!db_.open()) {
        qCCritical(SQLite,) << "Unable to open SQLITE database" << filename;
        return false;
    }

    // Create tables if they do not exist yet.
    auto query = QSqlQuery(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS property_history (id STRING, timestamp INTEGER, value BLOB)") ||
        !query.exec("CREATE UNIQUE INDEX IF NOT EXISTS property_history_index ON property_history (id, timestamp)") ||
        !query.exec("CREATE TABLE IF NOT EXISTS device_message (timestamp INTEGER, access_id STRING, device_id STRING, message_id INTEGER, message STRING)")) {

        qCCritical(SQLite,) << "Error during database initialization:" << query.lastError().text();
        return false;
    }

    timerEvent(nullptr);
    startTimer(cleanupInterval);

    return true;
}

bool SQLiteStorage::storePropertyValues_(const QMap<SIGlobalPropertyID, QVariant>& properties, const QDateTime& timestamp) {
    // If the timestamp is outside the time window to store data for, discard it.
    if (timestamp < QDateTime::currentDateTime().addDays(-maximalStorageDays_)) {
        qCWarning(SQLite,) << "Timestamp outside storage time windows, skipping";
        return true;
    }

    // Start transaction.
    db_.transaction();

    // Write value per value, fail and rollback transaction should a query fail.
    for (const auto& propertyID: properties.keys()) {
        auto query = QSqlQuery(db_);
        query.prepare("INSERT OR REPLACE INTO property_history (id, timestamp, value) VALUES (?, ?, ?)");
        query.addBindValue(propertyID.toString());
        query.addBindValue(timestamp.toSecsSinceEpoch());
        query.addBindValue(properties[propertyID]);
        if (!query.exec()) {
            qCCritical(SQLite,) << "Error during property value write:" << query.lastError().text();
            db_.rollback();
            return false;
        }
    }

    // Commit transaction.
    db_.commit();

    // Return success.
    qCDebug(SQLite,) << "Wrote" << properties.count() << "property value(s) to database";
    return true;
}

QVector<SIStorage::TimestampedProperty> SQLiteStorage::retrievePropertyValues_(const SIGlobalPropertyID& id, const QDateTime& from, const QDateTime& to, unsigned int limit) {
    QVector<TimestampedProperty> result;

    // Prepare query.
    auto query = QSqlQuery(db_);
    query.prepare("SELECT timestamp, value FROM property_history WHERE id = ? AND timestamp BETWEEN ? AND ? ORDER BY timestamp DESC limit ?");
    query.addBindValue(id.toString());
    query.addBindValue(std::max(from.toSecsSinceEpoch(), QDateTime::currentDateTime().addDays(-maximalStorageDays_).toSecsSinceEpoch()));
    query.addBindValue(to.toSecsSinceEpoch());
    query.addBindValue(limit);

    // Try to execute query, return empty list if query fails.
    // TODO: It might be better if the status could be returned.
    if (!query.exec()) {
        qCCritical(SQLite,) << "Error during property values read:" << query.lastError().text();
        return result;
    }

    // Add every row of the query result to the results vector.
    while (query.next()) {
        result.append({QDateTime::fromSecsSinceEpoch(query.value(0).toULongLong()), query.value(1)});
    }

    // Return the results.
    qCDebug(SQLite,) << "Read" << result.count() << "property value(s) for" << id.toString() << "from database";
    return result;
}

bool SQLiteStorage::storeDeviceMessages_(const QVector<SIDeviceMessage>& messages) {
    // Start transaction.
    db_.transaction();

    // Write message per message, fail and rollback transaction should a query fail.
    for (const auto& message: messages) {
        // If the timestamp is outside the time window to store data for, discard it.
        if (message.timestamp < QDateTime::currentDateTime().addDays(-maximalStorageDays_)) {
            qCWarning(SQLite,) << "Message timestamp outside storage time windows, skipping";
            continue;
        }

        auto query = QSqlQuery(db_);
        query.prepare("INSERT INTO device_message (timestamp, access_id, device_id, message_id, message) VALUES (?, ?, ?, ?, ?)");
        query.addBindValue(message.timestamp.toSecsSinceEpoch());
        query.addBindValue(message.accessID);
        query.addBindValue(message.deviceID);
        query.addBindValue(message.messageID);
        query.addBindValue(message.message);
        if (!query.exec()) {
            qCCritical(SQLite,) << "Error during message write:" << query.lastError().text();
            db_.rollback();
            return false;
        }
    }

    // Commit transaction.
    db_.commit();

    // Return success.
    qCDebug(SQLite,) << "Wrote" << messages.count() << "device message(s) to database";
    return true;
}

QVector<SIDeviceMessage> SQLiteStorage::retrieveDeviceMessages_(const QDateTime& from, const QDateTime& to, unsigned int limit) {
    QVector<SIDeviceMessage> result;

    // Prepare query.
    auto query = QSqlQuery(db_);
    query.prepare("SELECT timestamp, access_id, device_id, message_id, message FROM device_message WHERE timestamp BETWEEN ? AND ? ORDER BY timestamp DESC limit ?");
    query.addBindValue(std::max(from.toSecsSinceEpoch(), QDateTime::currentDateTime().addDays(-maximalStorageDays_).toSecsSinceEpoch()));
    query.addBindValue(to.toSecsSinceEpoch());
    query.addBindValue(limit);

    // Try to execute query, return empty list if query fails.
    // TODO: It might be better if the status could be returned.
    if (!query.exec()) {
        qCCritical(SQLite,) << "Error during messages read:" << query.lastError().text();
        return result;
    }

    // Add every row of the query result to the results vector.
    while (query.next()) {
        result.append({
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toUInt(),
            query.value(4).toString(),
            QDateTime::fromSecsSinceEpoch(query.value(0).toULongLong())
        });
    }

    // Return the results.
    qCDebug(SQLite,) << "Read" << result.count() << "device message(s) from database";
    return result;
}

void SQLiteStorage::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event)

    qCDebug(SQLite,) << "Database cleanup";

    {
        // Prepare query and bind values.
        QSqlQuery query("DELETE FROM property_history WHERE timestamp < ?", db_);
        query.addBindValue(QDateTime::currentDateTime().addDays(-maximalStorageDays_).toSecsSinceEpoch());

        // Execute query.
        if (query.exec()) {
            qCDebug(SQLite,) << "Removed" << query.numRowsAffected() << "values from property history";
        } else {
            qCCritical(SQLite,) << "Error during property history cleanup" << query.lastError().text();
        }
    }

    {
        // Prepare query and bind values.
        QSqlQuery query("DELETE FROM device_message WHERE timestamp < ?", db_);
        query.addBindValue(QDateTime::currentDateTime().addDays(maximalStorageDays_).toSecsSinceEpoch());

        // Execute query.
        if (query.exec()) {
            qCDebug(SQLite,) << "Removed" << query.numRowsAffected() << "values from device message storage";
        } else {
            qCCritical(SQLite,) << "Error during device message storage cleanup" << query.lastError().text();
        }
    }
}
