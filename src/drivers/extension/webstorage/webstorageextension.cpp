#include "webstorageextension.h"
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDir>
#include <QSqlError>
#include <algorithm>

Q_DECLARE_LOGGING_CATEGORY(WebStorage)

WebStorageExtension::WebStorageExtension(): SIExtension("WebStorage") {}

WebStorageExtension::~WebStorageExtension() {
    db_.close();
}

bool WebStorageExtension::open(const QString& filename) {
    // Check if SQLite driver is available.
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qCCritical(WebStorage,) << "Qt SQLITE driver not available";
        return false;
    }

    // Ensure that the folder exists and check if the database file already exists.
    QFileInfo fileInfo(filename);
    if (!fileInfo.exists()) {
        auto folder = fileInfo.absoluteDir();
        if (!folder.exists()) {
            qCWarning(WebStorage,) << "Folder" << folder.absolutePath() << "does not exist, trying to create it";
            if (!folder.mkpath(".")) {
                qCCritical(WebStorage,) << "Failed to create folder" << folder.absolutePath();
                return false;
            }
        }
        qCInfo(WebStorage,) << "Creating new SQLite database file" << filename;
    } else {
        qCInfo(WebStorage,) << "Opening existing SQLite database file" << filename;
    }

    // Create database.
    db_ = QSqlDatabase::addDatabase("QSQLITE", "org.sgw.SIExtensionDriver.webdata");
    if (!db_.isValid()) {
        qCCritical(WebStorage,) << "Unable to create SQLITE database";
        return false;
    }

    // Open database file.
    db_.setDatabaseName(filename);
    if (!db_.open()) {
        qCCritical(WebStorage,) << "Unable to open SQLITE database" << filename;
        return false;
    }

    // Create tables if they do not exist yet.
    auto query = QSqlQuery(db_);
    if (!query.exec("CREATE TABLE IF NOT EXISTS web_data (key STRING, value BLOB)") ||
        !query.exec("CREATE UNIQUE INDEX IF NOT EXISTS web_data_index ON web_data (key)") ) {

        qCCritical(WebStorage,) << "Error during database initialization:" << query.lastError().text();
        return false;
    }

    return true;
}

QStringList& WebStorageExtension::commands_() const {
    static QStringList commands_ {"write", "read"};
    return commands_;
}

SIExtension::Result WebStorageExtension::runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
    Q_UNUSED(context);

    if (command == "write") {
        return write_(headers.value("key"), body);
    } else if (command == "read") {
        return read_(headers.value("key"));
    } else {
        return {Status::UnsupportedCommand};
    }
}

SIExtension::Result WebStorageExtension::write_(const QString& key, const QByteArray& body) {
    if (key.isEmpty()) {
        return {Status::InvalidHeaders};
    }

    auto query = QSqlQuery(db_);
    query.prepare("INSERT OR REPLACE INTO web_data (key, value) VALUES (?, ?)");
    query.addBindValue(key);
    query.addBindValue(body);
    if (!query.exec()) {
        qCCritical(WebStorage,) << "Error during web data write:" << query.lastError().text();
        db_.rollback();
        return {Status::Error};
    }

    return {Status::Success, {{"key", key}}};
}

SIExtension::Result WebStorageExtension::read_(const QString& key) {
    if (key.isEmpty()) {
        return {Status::InvalidHeaders};
    }

    auto query = QSqlQuery(db_);
    query.prepare("SELECT value FROM web_data WHERE key = ?");
    query.addBindValue(key);
    if (!query.exec()) {
        qCCritical(WebStorage,) << "Error during web data read:" << query.lastError().text();
        return {Status::Error};
    }

    QByteArray body;
    if (query.next()) {
        body = query.value(0).toByteArray();
    }

    return {Status::Success, {{"key", key}}, body};
}
