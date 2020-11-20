#include "sideviceaccessregistry.h"
#include "sideviceaccess.h"
#include <cassert>
#include <QJsonArray>
#include <QJsonDocument>

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

    QJsonObject description {
        {"accesses", accesses}
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

void SIDeviceAccessRegistry::registerDeviceAccess_(SIDeviceAccess* access) {
    assert(access != nullptr);
    access->setParent(this);
}
