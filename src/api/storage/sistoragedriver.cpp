#include "sistoragedriver.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

SIStorageDriver* SIStorageDriver::loadStorageDriver(const QStringList& driverSearchPaths, QString driverName) {
    if (!driverName.endsWith(".ssd")) {
        driverName.append(".ssd");
    }
    for (const auto& driverSearchPath: driverSearchPaths) {
        QDir directory {driverSearchPath};
        if (directory.exists() && directory.exists(driverName)) {
            QPluginLoader pluginLoader(directory.absoluteFilePath(driverName));
            if (pluginLoader.load()) {
                return qobject_cast<SIStorageDriver*>(pluginLoader.instance());
            }
        }
    }

    return nullptr;
}
