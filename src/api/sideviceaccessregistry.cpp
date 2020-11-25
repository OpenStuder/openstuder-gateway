#include "sideviceaccessregistry.h"
#include "sideviceaccess.h"
#include "sideviceaccessdriver.h"
#include <cassert>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QPluginLoader>

struct SIDeviceAccessFactoryElement {
    QString name;
    QJsonObject metaData;
    SIDeviceAccessDriver* factory;
};

static QVector<SIDeviceAccessFactoryElement> factories_;

struct SIDeviceAccessRegistry::Private_ {

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

QJsonObject SIDeviceAccessRegistry::jsonDescription(SIJsonFlags flags) const {
    QJsonArray accesses;
    for (auto* child: children()) {
        auto* access = qobject_cast<SIDeviceAccess*>(child);
        accesses.append(access->jsonDescription(flags));
    }

    QJsonObject factories;
    for (const auto& factory: factories_) {
        factories[factory.name] = factory.metaData;
    }

    QJsonObject description {
        {"instances", accesses},
        {"drivers",  factories}
    };

    return description;
}

SIDeviceAccessRegistry& SIDeviceAccessRegistry::sharedRegistry() {
    static SIDeviceAccessRegistry* instance = nullptr;
    if (instance == nullptr) {
        instance = new SIDeviceAccessRegistry;
    }
    return *instance;
}

bool SIDeviceAccessRegistry::registerDeviceAccessDriver(const QString& name, const QJsonObject& metaData, SIDeviceAccessDriver* deviceAccessFactory) {
    if (deviceAccessFactory && std::none_of(factories_.cbegin(), factories_.cend(), [&name](const SIDeviceAccessFactoryElement& pluginElement) {
        return pluginElement.name == name;
    })) {
        factories_.append({name, metaData, deviceAccessFactory});
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
        auto* plugin = qobject_cast<SIDeviceAccessDriver*>(pluginLoader.instance());
        return registerDeviceAccessDriver(name, metaData["MetaData"].toObject(), plugin);
    }
    return false;
}

int SIDeviceAccessRegistry::loadDeviceAccessDriversInFolder(const QString& driversFolderPath) {
    int pluginsLoaded = 0;
    QDir pluginFolder(driversFolderPath);
    for (const auto& pluginFile: pluginFolder.entryList({"*.dap"})) {
        if (loadDeviceAccessDriver(pluginFolder.filePath(pluginFile))) {
            ++pluginsLoaded;
        }
    }
    return pluginsLoaded;
}

void SIDeviceAccessRegistry::registerDeviceAccessInstance_(SIDeviceAccess* access) {
    assert(access != nullptr);
    access->setParent(this);
}
