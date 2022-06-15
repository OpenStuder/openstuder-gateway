#include <QDir>
#include <QPluginLoader>
#include "siextensiondriver.h"

SIExtensionDriver* SIExtensionDriver::loadExtensionDriver(const QStringList& driverSearchPaths, QString driverName) {
    if (!driverName.endsWith(".sed")) {
        driverName.append(".sed");
    }
    for (const auto& driverSearchPath: driverSearchPaths) {
        QDir directory {driverSearchPath};
        if (directory.exists() && directory.exists(driverName)) {
            QPluginLoader pluginLoader(directory.absoluteFilePath(driverName));
            if (pluginLoader.load()) {
                return qobject_cast<SIExtensionDriver*>(pluginLoader.instance());
            }
        }
    }

    return nullptr;
}
