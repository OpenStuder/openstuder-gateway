#include "siextensionmanager.h"

#include <utility>

class SIExtensionManager::Context: public SIExtensionContext {
  public:
    Context(QString userName, SIAccessLevel accessLevel, SIUserAuthorizer* userAuthorizer)
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

QStringList SIExtensionManager::availableExtensions() const {
    return extensions_.keys();
}

SIExtension::Result
SIExtensionManager::callExtension(const QString& extension, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body, SIContext& context, SISessionContext& sessionCtx) {
    Context extensionContext(sessionCtx.username(), sessionCtx.accessLevel(), context.userAuthorizer());
    auto* extensionInstance = extensions_.value(extension);
    if (extensionInstance == nullptr) {
        return {SIExtension::Status::UnsupportedExtension, {{"extension", extension}}};
    }
    return extensionInstance->runCommand(extensionContext, command, headers, body);
}

void SIExtensionManager::addExtension(SIExtension* extension) {
    if (extension == nullptr) { return; }
    extension->setParent(this);
    extensions_.insert(extension->id(), extension);
}
