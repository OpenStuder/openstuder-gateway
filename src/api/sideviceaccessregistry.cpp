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

SIDeviceAccessRegistry::SIDeviceAccessRegistry(): priv_(new Private_) {}

SIDeviceAccessRegistry::~SIDeviceAccessRegistry() = default;

int SIDeviceAccessRegistry::deviceAccessCount() const {
    return children().count();
}

QPointer<SIDeviceAccess> SIDeviceAccessRegistry::deviceAccess(int index) const {
    if (index < children().count()) {
        return qobject_cast<SIDeviceAccess*>(children()[index]);
    } else {
        return nullptr;
    }
}

QPointer<SIDeviceAccess> SIDeviceAccessRegistry::deviceAccess(const QString& id) const {
    for (auto* child: children()) {
        auto* access = qobject_cast<SIDeviceAccess*>(child);
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

    priv_->instanceDriverNames[instance] = driverName;
    instance->setParent(this);
    return true;
}

QJsonObject SIDeviceAccessRegistry::jsonDescription(SIJsonFlags flags) const {
    QJsonObject description;

    if (flags.testFlag(SIJsonFlag::IncludeAccessInformation)) {
        QJsonArray instances;
        for (auto* child: children()) {
            auto* access = qobject_cast<SIDeviceAccess*>(child);
            QJsonObject description = access->jsonDescription(flags);
            description["driver"] = priv_->instanceDriverNames[access];
            instances.append(description);
        }
        description["instances"] = instances;
    }

    if (flags.testFlag(SIJsonFlag::IncludeDriverInformation)) {
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

bool SIDeviceAccessRegistry::loadDeviceAccessDriver(const QString& driverFile) {
    QPluginLoader pluginLoader(driverFile);
    if (pluginLoader.load()) {
        auto metaData = pluginLoader.metaData();
        auto name = metaData["MetaData"].toObject()["name"].toString();
        auto* driver = qobject_cast<SIDeviceAccessDriver*>(pluginLoader.instance());
        return registerDeviceAccessDriver(name, metaData["MetaData"].toObject(), driver);
    }
    return false;
}

int SIDeviceAccessRegistry::loadDeviceAccessDriversInFolder(const QString& driversFolderPath) {
    int driversLoaded = 0;
    QDir driversFolder(driversFolderPath);
    for (const auto& driverFile: driversFolder.entryList({"*.dap"})) {
        if (loadDeviceAccessDriver(driversFolder.filePath(driverFile))) {
            ++driversLoaded;
        }
    }
    return driversLoaded;
}
