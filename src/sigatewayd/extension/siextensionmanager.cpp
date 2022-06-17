#include "siextensionmanager.h"

#include <utility>

class SIExtensionManager::Context_: public SIExtensionContext {
  public:
    Context_(QString userName, SIAccessLevel accessLevel, SIUserAuthorizer* userAuthorizer)
        : sessionUserName_(std::move(userName)), sessionAccessLevel_(accessLevel), userAuthorizer_(userAuthorizer) {}

    QString sessionUserName() const override {
        return sessionUserName_;
    }
    SIAccessLevel sessionAccessLevel() const override {
        return sessionAccessLevel_;
    }
    SIUserAuthorizer* userAuthorizer() const override {
        return userAuthorizer_;
    }

  private:
    QString sessionUserName_;
    SIAccessLevel sessionAccessLevel_;
    SIUserAuthorizer* userAuthorizer_;
};

QStringList SIExtensionManager::availableExtensions(const std::function<bool (const SIExtension*)>& predicate) const {
    if (predicate) {
        QStringList filtered;
        for (auto* extension: extensions_) {
            if (predicate(extension)) {
                filtered.append(extension->id());
            }
        }
        return filtered;
    } else {
        return extensions_.keys();
    }
}

const SIExtensionWebSocketResult*
SIExtensionManager::callExtension(const QString& extension, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body, SIContext& context, SISessionContext& sessionCtx) {
    Context_ extensionContext(sessionCtx.username(), sessionCtx.accessLevel(), context.userAuthorizer());
    auto* extensionInstance = extensions_.value(extension);
    if (extensionInstance == nullptr) {
        return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::UnsupportedExtension);
    }
    return extensionInstance->runCommand(extensionContext, command, headers, body);
}

const SIExtensionBluetoothResult* SIExtensionManager::callExtension(const QString& extension, const QString& command, const QVector<QVariant>& parameters, SIContext& context, SISessionContext& sessionCtx) {
    Context_ extensionContext(sessionCtx.username(), sessionCtx.accessLevel(), context.userAuthorizer());
    auto* extensionInstance = extensions_.value(extension);
    if (extensionInstance == nullptr || !extensionInstance->bluetoothSupported()) {
        return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::UnsupportedExtension);
    }
    return extensionInstance->runCommand(extensionContext, command, parameters);
}

void SIExtensionManager::addExtension(SIExtension* extension) {
    if (extension == nullptr) { return; }
    extension->setParent(this);
    extensions_.insert(extension->id(), extension);
}
