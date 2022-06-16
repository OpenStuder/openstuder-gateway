#include "usermanagementextension.h"
#include <QLoggingCategory>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDir>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>

Q_DECLARE_LOGGING_CATEGORY(UserManagement)

UserManagementExtension::UserManagementExtension(QStringList adminUsers): SIExtension("UserManagement"), adminUsers_(std::move(adminUsers)) {}

UserManagementExtension::~UserManagementExtension() = default;

SIExtension::Result UserManagementExtension::list_(const SIExtensionContext& context) {
    bool ok;
    auto users = context.userAuthorizer()->listUsers(&ok);

    if (!ok) {
        return {Status::Error};
    }

    QJsonObject usersObj;
    for (const auto& key: users.keys()) {
        usersObj.insert(key, to_string(users[key]));
    }
    return {SIExtension::Status::Success, {},
                  QJsonDocument(usersObj).toJson(QJsonDocument::Compact)};
}

SIExtension::Result UserManagementExtension::add_(const SIExtensionContext& context, const QString& username, const QString& password, SIAccessLevel accessLevel) {
    if (username.isEmpty() || password.isEmpty() || accessLevel < SIAccessLevel::None || accessLevel > SIAccessLevel::QualifiedServicePersonnel) {
        return {SIExtension::Status::InvalidHeaders};
    }

    if (context.userAuthorizer()->addUser(username, password, accessLevel)) {
        return {Status::Success};
    } else {
        return {Status::Error};
    }
}

SIExtension::Result UserManagementExtension::changePassword_(const SIExtensionContext& context, const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        return {SIExtension::Status::InvalidHeaders};
    }

    if (context.userAuthorizer()->changeUserPassword(username, password)) {
        return {Status::Success};
    } else {
        return {Status::Error};
    }
}

SIExtension::Result UserManagementExtension::changeAccessLevel_(const SIExtensionContext& context, const QString& username, SIAccessLevel accessLevel) {
    if (username.isEmpty() || accessLevel < SIAccessLevel::None || accessLevel > SIAccessLevel::QualifiedServicePersonnel) {
        return {SIExtension::Status::InvalidHeaders};
    }

    if (context.userAuthorizer()->changeUserAccessLevel(username, accessLevel)) {
        return {Status::Success};
    } else {
        return {Status::Error};
    }
}

SIExtension::Result UserManagementExtension::remove_(const SIExtensionContext& context, const QString& username) {
    if (username.isEmpty()) {
        return {SIExtension::Status::InvalidHeaders};
    }

    if (context.userAuthorizer()->removeUser(username)) {
        return {Status::Success};
    } else {
        return {Status::Error};
    }
}

QStringList& UserManagementExtension::commands_() const {
    static QStringList commands_ {"list", "add", "change-password", "change-access-level", "remove"};
    return commands_;
}

SIExtension::Result UserManagementExtension::runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
    Q_UNUSED(body);
    qCCritical(UserManagement,) << context.sessionUserName();
    qCCritical(UserManagement,) << adminUsers_;
    if (!adminUsers_.contains(context.sessionUserName())) {
        return {Status::Forbidden};
    }

    if (context.userAuthorizer() == nullptr || !context.userAuthorizer()->managementSupported()) {
        return {Status::Error};
    }

    if (command == "list") {
        return list_(context);
    } else if (command == "add") {
        return add_(context, headers["username"], headers["password"], SIAccessLevelFromString(headers["access_level"]));
    } else if (command == "change_password") {
        return changePassword_(context, headers["username"], headers["password"]);
    } else if (command == "change_access_level") {
        return changeAccessLevel_(context, headers["username"], SIAccessLevelFromString(headers["access_level"]));
    } else if (command == "remove") {
        return remove_(context, headers["username"]);
    } else {
        return {Status::UnsupportedCommand};
    }
}
