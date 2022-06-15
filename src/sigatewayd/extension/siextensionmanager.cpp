#include "siextensionmanager.h"

QStringList SIExtensionManager::availableExtensions() const {
    return extensions_.keys();
}

SIExtension::Result SIExtensionManager::callExtension(const QString& extension, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body, SIContext& context) {
    Q_UNUSED(context);
    auto* extensionInstance = extensions_.value(extension);
    if (extensionInstance == nullptr) {
        return {SIExtension::Status::UnsupportedExtension, {{"extension", extension}}};
    }
    return extensionInstance->runCommand(command, headers, body);
 }

void SIExtensionManager::addExtension(SIExtension* extension) {
    if (extension == nullptr) return;
    extension->setParent(this);
    extensions_.insert(extension->id(), extension);
}
