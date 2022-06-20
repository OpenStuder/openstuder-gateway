#include "siextension.h"
#include <algorithm>

struct SIExtension::Private_ {
    QString id;
    QStringList allowedUsers;
};

SIExtension::SIExtension(const QString& id, const QStringList allowedUsers): private_(new Private_) {
    private_->id = id;
    private_->allowedUsers = allowedUsers;
}

SIExtension::~SIExtension() = default;

void SIExtension::setAllowedUsers(const QStringList& allowedUsers) {
    private_->allowedUsers = allowedUsers;
}

const QString& SIExtension::id() const {
    return private_->id;
}

const QStringList& SIExtension::commands() const {
    return commands_();
}

const SIExtensionWebSocketResult* SIExtension::runCommand(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
    if (!private_->allowedUsers.isEmpty() && !private_->allowedUsers.contains(context.sessionUserName())) {
        return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::Forbidden);
    }

    auto result = runCommand_(context, command, headers, body);

    if (result != nullptr) {
        result->extension_ = id();
        result->command_ = command;
    }

    return result;
}

bool SIExtension::bluetoothSupported() const {
    return bluetoothSupported_();
}

const SIExtensionBluetoothResult* SIExtension::runCommand(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters) {
    if (!private_->allowedUsers.isEmpty() && !private_->allowedUsers.contains(context.sessionUserName())) {
        return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::Forbidden);
    }

    auto result = runCommand_(context, command, parameters);

    if (result != nullptr) {
        result->extension_ = id();
        result->command_ = command;
    }

    return result;
}

bool SIExtension::validateWebSocketHeaders(const QMap<QString, QString>& headers, const std::initializer_list<const char*>& required, const std::initializer_list<const char*>& optional) {
    auto keys = headers.keys();

    // Check that all required headers are present.
    for (const auto& key: required) {
        if (!keys.contains(key)) return false;
        keys.removeOne(key);
    }

    // Check that no other than option headers are present.
    for (const auto& key: optional) {
        keys.removeOne(key);
    }
    return keys.isEmpty();
}

bool SIExtension::validateBluetoothParameters(const QVector<QVariant>& parameters, const QVector<QSet<QVariant::Type>>& parameterTypes, bool allowAdditionalParameters) {
    if (allowAdditionalParameters) {
        if (parameters.count() < parameterTypes.count()) {
            return false;
        }
    } else {
        if (parameters.count() != parameterTypes.count()) {
            return false;
        }
    }

    for (int i = 0; i < parameterTypes.count(); ++i) {
        if (!parameterTypes[i].isEmpty() && std::none_of(parameterTypes[i].cbegin(), parameterTypes[i].cend(), [&](QVariant::Type type) { return parameters[i].canConvert(type); })) {
            return false;
        }
    }

    return true;
}
