#include "siuserauthorizedriver.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

SIUserAuthorizeDriver* SIUserAuthorizeDriver::loadUserAuthorizeDriver(const QStringList& driverSearchPaths, QString driverName) {
    if (!driverName.endsWith(".sud")) {
        driverName.append(".sud");
    }
    for (const auto& driverSearchPath: driverSearchPaths) {
        QDir directory {driverSearchPath};
        if (directory.exists() && directory.exists(driverName)) {
            QPluginLoader pluginLoader(directory.absoluteFilePath(driverName));
            if (pluginLoader.load()) {
                return qobject_cast<SIUserAuthorizeDriver*>(pluginLoader.instance());
            }
        }
    }

    return nullptr;
}
