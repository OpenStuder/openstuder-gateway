#include "sideviceaccessregistry.h"
#include "sideviceaccess.h"
#include "sideviceaccessdriver.h"
#include <algorithm>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QPluginLoader>

struct SIDeviceAccessDriverElement {
    QString name;
    QJsonObject metaData;
    SIDeviceAccessDriver* driver;
};

static QVector<SIDeviceAccessDriverElement> drivers_;

struct SIDeviceAccessRegistry::Private_ {
    QMap<SIDeviceAccess*,QString> instanceDriverNames;
};

SIDeviceAccessRegistry::SIDeviceAccessRegistry(): private_(new Private_) {}

SIDeviceAccessRegistry::~SIDeviceAccessRegistry() = default;

int SIDeviceAccessRegistry::enumerateDevices() {
    int count = 0;
    for (auto* child: children()) {
#ifdef Q_OS_MACOS
        auto* access = reinterpret_cast<SIDeviceAccess*>(child);
#else
        auto* access = qobject_cast<SIDeviceAccess*>(child);
#endif
        count += access->enumerateDevices();
    }
    return count;
}

int SIDeviceAccessRegistry::deviceAccessCount() const {
    return children().count();
}

QPointer<SIDeviceAccess> SIDeviceAccessRegistry::deviceAccess(int index) const {
    if (index < children().count()) {
#ifdef Q_OS_MACOS
        return reinterpret_cast<SIDeviceAccess*>(children()[index]);
#else
        return qobject_cast<SIDeviceAccess*>(children()[index]);
#endif
    } else {
        return nullptr;
    }
}

QPointer<SIDeviceAccess> SIDeviceAccessRegistry::deviceAccess(const QString& id) const {
    for (auto* child: children()) {
#ifdef Q_OS_MACOS
        auto access = reinterpret_cast<SIDeviceAccess*>(child);
#else
        auto* access = qobject_cast<SIDeviceAccess*>(child);
#endif
        if (access->id() == id) {
            return access;
        }
    }
    return nullptr;
}

bool SIDeviceAccessRegistry::instantiateDeviceAccess(const QString& driverName, const QString& id, const QVariantMap& parameters) {
    auto driver = std::find_if(drivers_.cbegin(), drivers_.cend(), [&driverName](const SIDeviceAccessDriverElement& driverElement) {
        return driverElement.name == driverName;
    });

    if (driver == drivers_.cend()) {
        return false;
    }

    auto* instance = driver->driver->createDeviceAccessInstance(id, parameters);
    if (instance == nullptr) {
        return false;
    }

    private_->instanceDriverNames[instance] = driverName;
    instance->setParent(this);
    return true;
}

QJsonObject SIDeviceAccessRegistry::jsonDescription(SIAccessLevel accessLevel, SIDescriptionFlags flags) const {
    QJsonObject description;

    if (flags.testFlag(SIDescriptionFlag::IncludeAccessInformation)) {
        QJsonArray instances;
        for (auto* child: children()) {
#ifdef Q_OS_MACOS
            auto access = reinterpret_cast<SIDeviceAccess*>(child);
#else
            auto* access = qobject_cast<SIDeviceAccess*>(child);
#endif
            QJsonObject accessDescription = access->jsonDescription(accessLevel, flags);
            accessDescription["driver"] = private_->instanceDriverNames[access];
            instances.append(accessDescription);
        }
        description["instances"] = instances;
    }

    if (flags.testFlag(SIDescriptionFlag::IncludeDriverInformation)) {
        QJsonObject drivers;
        for (const auto& driver: drivers_) {
            drivers[driver.name] = driver.metaData;
        }
        description["drivers"] = drivers;
    }

    return description;
}

SIDeviceAccessRegistry& SIDeviceAccessRegistry::sharedRegistry() {
    static SIDeviceAccessRegistry* instance = nullptr;
    if (instance == nullptr) {
        instance = new SIDeviceAccessRegistry;
    }
    return *instance;
}

bool SIDeviceAccessRegistry::registerDeviceAccessDriver(const QString& name, const QJsonObject& metaData, SIDeviceAccessDriver* deviceAccessDriver) {
    if (deviceAccessDriver && std::none_of(drivers_.cbegin(), drivers_.cend(), [&name](const SIDeviceAccessDriverElement& driverElement) {
        return driverElement.name == name;
    })) {
        drivers_.append({name, metaData, deviceAccessDriver});
        return true;
    } else {
        return false;
    }
}

bool SIDeviceAccessRegistry::loadDeviceAccessDriver(QString driverFile) {
    if (!driverFile.endsWith(".sdd")) {
        driverFile.append(".sdd");
    }
    auto name = QFileInfo(driverFile).baseName();
    if (hasDeviceAccessDriver_(name)) {
        return true;
    }
    QPluginLoader pluginLoader(driverFile);
    if (pluginLoader.load()) {
        auto metaData = pluginLoader.metaData();
        auto* driver = qobject_cast<SIDeviceAccessDriver*>(pluginLoader.instance());
        if (registerDeviceAccessDriver(name, metaData["MetaData"].toObject(), driver)) {
            return true;
        } else {
            pluginLoader.unload();
            return false;
        }
    }
    return false;
}

bool SIDeviceAccessRegistry::loadDeviceAccessDriver(const QStringList& driverSearchPaths, QString driverName) {
    if (!driverName.endsWith(".sdd")) {
        driverName.append(".sdd");
    }
    for (const auto& driverSearchPath: driverSearchPaths) {
        QDir directory {driverSearchPath};
        if (directory.exists() && directory.exists(driverName)) {
            return loadDeviceAccessDriver(directory.absoluteFilePath(driverName));
        }
    }
    return false;
}

int SIDeviceAccessRegistry::loadDeviceAccessDriversInFolder(const QString& driversFolderPath) {
    int driversLoaded = 0;
    QDir driversFolder(driversFolderPath);
    for (const auto& driverFile: driversFolder.entryList({"*.sdd"})) {
        if (loadDeviceAccessDriver(driversFolder.filePath(driverFile))) {
            ++driversLoaded;
        }
    }
    return driversLoaded;
}

bool SIDeviceAccessRegistry::hasDeviceAccessDriver_(const QString& name) {
    return std::any_of(drivers_.cbegin(), drivers_.cend(), [&name](const SIDeviceAccessDriverElement& driverElement) {
        return driverElement.name == name;
    });
}
