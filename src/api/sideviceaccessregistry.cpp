#include "sideviceaccessregistry.h"
#include "sideviceaccess.h"
#include <QJsonArray>
#include <QJsonDocument>

struct SIDeviceAccessRegistry::Private_ {
    QVector<std::shared_ptr<SIDeviceAccess>> accesses;
};

SIDeviceAccessRegistry::SIDeviceAccessRegistry(): priv_(new Private_) {}

SIDeviceAccessRegistry::~SIDeviceAccessRegistry() = default;

int SIDeviceAccessRegistry::deviceAccessCount() const {
    return priv_->accesses.count();
}

std::weak_ptr<SIDeviceAccess> SIDeviceAccessRegistry::deviceAccess(int index) const {
    if (index < priv_->accesses.count()) {
        return priv_->accesses[index];
    } else {
        return {};
    }
}

std::weak_ptr<SIDeviceAccess> SIDeviceAccessRegistry::deviceAccess(const QString& id) const {
    for (const auto& access: priv_->accesses) {
        if (access->id() == id) {
            return access;
        }
    }
    return {};
}

QJsonObject SIDeviceAccessRegistry::jsonDescription(SIJsonFlags flags) const {
    QJsonArray accesses;
    for (const auto& access: priv_->accesses) {
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
    priv_->accesses.append(std::shared_ptr<SIDeviceAccess>(access));
}
