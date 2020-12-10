#include "sistoragedriver.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

SIStorageDriver* SIStorageDriver::loadStorageDriver(const QStringList& driverSearchPaths, QString driverFile) {
    driverFile.append(".ssd");
    for (const auto& driverSearchPath: driverSearchPaths) {
        QDir directory {driverSearchPath};
        if (directory.exists() && directory.exists(driverFile)) {
            QPluginLoader pluginLoader(directory.absoluteFilePath(driverFile));
            if (pluginLoader.load()) {
                return qobject_cast<SIStorageDriver*>(pluginLoader.instance());
            }
        }
    }

    return nullptr;
}
