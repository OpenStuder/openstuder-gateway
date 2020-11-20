#include "sideviceaccess.h"
#include "sidevice.h"
#include "sideviceaccessregistry.h"
#include <QJsonArray>

struct SIDeviceAccess::Private_ {
    QString id;
    QVector<std::shared_ptr<SIDevice>> devices;
    mutable SIJsonFlags cachedJsonFlags = SIJsonFlag::None;
    mutable QJsonObject cachedJsonDescription;
};

SIDeviceAccess::SIDeviceAccess(const QString& id, SIDeviceAccessRegistry* registry): priv_(new Private_) {
    priv_->id = id;
    if (registry != nullptr) {
        registry->registerDeviceAccess_(this);
    } else {
        SIDeviceAccessRegistry::sharedRegistry().registerDeviceAccess_(this);
    }
}

SIDeviceAccess::~SIDeviceAccess() = default;

const QString& SIDeviceAccess::id() const {
    return priv_->id;
}

int SIDeviceAccess::enumerateDevices() {
    priv_->cachedJsonDescription = {};
    if (enumerateDevices_(priv_->devices)) {
        return priv_->devices.count();
    } else
    {
        return -1;
    }
}

int SIDeviceAccess::deviceCount() const {
    return priv_->devices.count();
}

std::weak_ptr<SIDevice> SIDeviceAccess::device(int index) const {
    if (index < priv_->devices.count()) {
        return priv_->devices[index];
    } else {
        return {};
    }
}

std::weak_ptr<SIDevice> SIDeviceAccess::device(const QString& id) const {
    for (const auto& device: priv_->devices) {
        if (device->id() == id) {
            return device;
        }
    }
    return {};
}

const QJsonObject& SIDeviceAccess::jsonDescription(SIJsonFlags flags) const {
    if (priv_->cachedJsonFlags != flags) {
        priv_->cachedJsonDescription = {};
    }
    priv_->cachedJsonFlags = flags;

    if (priv_->cachedJsonDescription.isEmpty()) {
        priv_->cachedJsonDescription["id"] = id();
        if (flags.testFlag(SIJsonFlag::IncludeAccessDetails)) {
            QJsonArray devs;
            for (const auto& device: priv_->devices) {
                devs.append(device->jsonDescription(flags));
            }
            priv_->cachedJsonDescription["devices"] = devs;
            completeJsonDescription_(priv_->cachedJsonDescription, flags);
        }
    }
    return priv_->cachedJsonDescription;
}

void SIDeviceAccess::completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const {
    Q_UNUSED(object)
    Q_UNUSED(flags)
}
