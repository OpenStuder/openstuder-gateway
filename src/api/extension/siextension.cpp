#include "siextension.h"

struct SIExtension::Private_ {
    QString id;
};

SIExtension::SIExtension(const QString& id): private_(new Private_) {
    private_->id = id;
}

SIExtension::~SIExtension() = default;

const QString& SIExtension::id() const {
    return private_->id;
}

const QStringList& SIExtension::commands() const {
    return commands_();
}

const SIExtensionWebSocketResult* SIExtension::runCommand(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
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
    auto result = runCommand_(context, command, parameters);

    if (result != nullptr) {
        result->extension_ = id();
        result->command_ = command;
    }

    return result;
}
