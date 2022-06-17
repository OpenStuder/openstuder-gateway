#include "usermanagementextension.h"
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QJsonObject>

Q_DECLARE_LOGGING_CATEGORY(UserManagement)

UserManagementExtension::UserManagementExtension(QStringList allowedUsers): SIExtension("UserManagement"), allowedUsers_(std::move(allowedUsers)) {}

UserManagementExtension::~UserManagementExtension() = default;

QStringList& UserManagementExtension::commands_() const {
    static QStringList commands_ {"list", "add", "change-password", "change-access-level", "remove"};
    return commands_;
}

SIExtensionWebSocketResult* UserManagementExtension::runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
    Q_UNUSED(body);
    if (!allowedUsers_.contains(context.sessionUserName())) {
        return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Forbidden);
    }

    if (context.userAuthorizer() == nullptr || !context.userAuthorizer()->managementSupported()) {
        return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Error);
    }

    if (command == "list") {
        bool ok;
        auto users = context.userAuthorizer()->listUsers(&ok);
        if (!ok) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Error);
        }

        QJsonObject usersObj;
        for (const auto& key: users.keys()) {
            usersObj.insert(key, to_string(users[key]));
        }
        return new SIExtensionWebSocketResult(SIExtensionStatus::Success, {}, QJsonDocument(usersObj).toJson(QJsonDocument::Compact));

    } else if (command == "add") {
        auto username = headers["username"];
        auto password = headers["password"];
        auto accessLevel = SIAccessLevelFromString(headers["access_level"]);

        if (username.isEmpty() || password.isEmpty() || accessLevel < SIAccessLevel::None || accessLevel > SIAccessLevel::QualifiedServicePersonnel) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        if (context.userAuthorizer()->addUser(username, password, accessLevel)) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Error);
        }
    } else if (command == "change_password") {
        auto username = headers["username"];
        auto password = headers["password"];

        if (username.isEmpty() || password.isEmpty()) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        if (context.userAuthorizer()->changeUserPassword(username, password)) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Error);
        }
    } else if (command == "change_access_level") {
        auto username = headers["username"];
        auto accessLevel = SIAccessLevelFromString(headers["access_level"]);

        if (username.isEmpty() || accessLevel < SIAccessLevel::None || accessLevel > SIAccessLevel::QualifiedServicePersonnel) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        if (context.userAuthorizer()->changeUserAccessLevel(username, accessLevel)) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Error);
        }
    } else if (command == "remove") {
        auto username = headers["username"];

        if (username.isEmpty()) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        if (context.userAuthorizer()->removeUser(username)) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Error);
        }
    } else {
        return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::UnsupportedCommand);
    }
}
bool UserManagementExtension::bluetoothSupported_() const {
    return true;
}

SIExtensionBluetoothResult* UserManagementExtension::runCommand_(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters) {
    if (!allowedUsers_.contains(context.sessionUserName())) {
        return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Forbidden);
    }

    if (context.userAuthorizer() == nullptr || !context.userAuthorizer()->managementSupported()) {
        return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Error);
    }

    if (command == "list") {
        bool ok;
        auto users = context.userAuthorizer()->listUsers(&ok);
        if (!ok) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Error);
        }

        QVariantMap usersMap;
        for (const auto& key: users.keys()) {
            usersMap.insert(key, (unsigned int)users[key]);
        }
        return new SIExtensionBluetoothResult (SIExtensionStatus::Success, {{usersMap}});
    } else if (command == "add") {
        if (parameters.size() != 3) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }
        auto username = parameters[0].toString();
        auto password = parameters[1].toString();
        auto accessLevel = static_cast<SIAccessLevel>(parameters[2].toUInt());
        if (username.isEmpty() || password.isEmpty() || accessLevel < SIAccessLevel::None || accessLevel > SIAccessLevel::QualifiedServicePersonnel) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        if (context.userAuthorizer()->addUser(username, password, accessLevel)) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Error);
        }
    } else if (command == "change_password") {
        if (parameters.size() != 2) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }
        auto username = parameters[0].toString();
        auto password = parameters[1].toString();

        if (username.isEmpty() || password.isEmpty()) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        if (context.userAuthorizer()->changeUserPassword(username, password)) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Error);
        }
    } else if (command == "change_access_level") {
        if (parameters.size() != 2) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }
        auto username = parameters[0].toString();
        auto accessLevel = static_cast<SIAccessLevel>(parameters[2].toUInt());

        if (username.isEmpty() || accessLevel < SIAccessLevel::None || accessLevel > SIAccessLevel::QualifiedServicePersonnel) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        if (context.userAuthorizer()->changeUserAccessLevel(username, accessLevel)) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Error);
        }
    } else if (command == "remove") {
        if (parameters.size() != 1) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }
        auto username = parameters[0].toString();

        if (username.isEmpty()) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        if (context.userAuthorizer()->removeUser(username)) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Success);
        } else {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Error);
        }
    } else {
        return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::UnsupportedCommand);
    }
}
