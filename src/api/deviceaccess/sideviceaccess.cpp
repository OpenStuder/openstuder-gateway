#include "sideviceaccess.h"
#include "sidevice.h"
#include <QJsonArray>

struct SIDeviceAccess::Private_ {
    QString id;
};

SIDeviceAccess::SIDeviceAccess(const QString& id): priv_(new Private_) {
    priv_->id = id;
}

SIDeviceAccess::~SIDeviceAccess() = default;

const QString& SIDeviceAccess::id() const {
    return priv_->id;
}

QVector<SIDeviceMessage> SIDeviceAccess::retrievePendingDeviceMessages() const {
    QVector<SIDeviceMessage> messages;
    retrievePendingDeviceMessages_(messages);
    return messages;
}

int SIDeviceAccess::enumerateDevices() {
    // Populate device list.
    QVector<SIDevice*> existingDevices;
    for (const auto& child: children()) {
#ifdef Q_OS_MACOS
        existingDevices.append(reinterpret_cast<SIDevice*>(child));
#else
        devices.append(qobject_cast<SIDevice*>(child));
#endif
    }

    // Let the driver enumerate the devices.
    auto enumeratedDevices = existingDevices;
    if (enumerateDevices_(enumeratedDevices)) {

        // Send signals for all removed devices.
        for (auto* device: existingDevices) {
            if (!enumeratedDevices.contains(device)) {
                emit deviceRemoved(*device);
                delete device;
            }
        }

        // Send signal for all added devices and add them as children.
        for (auto* device: enumeratedDevices) {
            if (!existingDevices.contains(device)) {
                device->setParent(this);
                emit deviceAdded(*device);
            }
        }

        // Return the number of children.
        return children().count();
    } else {

        // Return error.
        return -1;
    }
}

int SIDeviceAccess::deviceCount() const {
    return children().count();
}

QPointer<SIDevice> SIDeviceAccess::device(int index) const {
    if (index < children().count()) {
#ifdef Q_OS_MACOS
        return reinterpret_cast<SIDevice*>(children()[index]);
#else
        return qobject_cast<SIDevice*>(children()[index]);
#endif
    } else {
        return nullptr;
    }
}

QPointer<SIDevice> SIDeviceAccess::device(const QString& id) const {
    for (auto* child: children()) {
#ifdef Q_OS_MACOS
        auto* device = reinterpret_cast<SIDevice*>(child);
#else
        auto* device = qobject_cast<SIDevice*>(child);
#endif
        if (device->id() == id) {
            return device;
        }
    }
    return nullptr;
}

QJsonObject SIDeviceAccess::jsonDescription(SIAccessLevel accessLevel, SIJsonFlags flags) const {
    QJsonObject description;
        description["id"] = id();
        if (flags.testFlag(SIJsonFlag::IncludeDeviceInformation)) {
            QJsonArray devs;
            for (auto* child: children()) {
#ifdef Q_OS_MACOS
                auto* device = reinterpret_cast<SIDevice*>(child);
#else
                auto* device = qobject_cast<SIDevice*>(child);
#endif
                devs.append(device->jsonDescription(accessLevel, flags));
            }
            description["devices"] = devs;
            completeJsonDescription_(description, flags);
        }

    return description;
}

void SIDeviceAccess::retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const {
    Q_UNUSED(messages)
}

void SIDeviceAccess::completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const {
    Q_UNUSED(object)
    Q_UNUSED(flags)
}
