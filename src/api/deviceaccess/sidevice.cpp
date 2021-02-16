#include "sidevice.h"
#include "sideviceaccess.h"
#include <QJsonArray>

struct SIDevice::Private_ {
    QString model;
    QString id;
};

SIDevice::SIDevice(const QString& model, const QString& id): priv_(new Private_) {
    priv_->model = model;
    priv_->id = id;
}

SIDevice::~SIDevice() = default;

const QString& SIDevice::model() const {
    return priv_->model;
}

const QString& SIDevice::id() const {
    return priv_->id;
}

const QVector<SIProperty>& SIDevice::properties() const {
    return properties_();
}

SIProperty SIDevice::property(SIPropertyID id) const {
    auto& properties = properties_();
    auto property = std::find_if(properties.cbegin(), properties.cend(), [&id](const SIProperty& p) { return p.id() == id; });
    if (property != properties.cend()) {
        return *property;
    } else {
        return {};
    }
}

SIPropertyReadResult SIDevice::readProperty(SIPropertyID id) const {
    return readProperty_(id);
}

QVector<SIPropertyReadResult> SIDevice::readProperties(const QVector<SIPropertyID>& ids) const {
    return readProperties_(ids);
}

SIPropertyWriteResult SIDevice::writeProperty(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) {
    return writeProperty_(id, value, flags);
}

QVector<SIPropertyWriteResult> SIDevice::writeProperties(const QVector<const QPair<SIPropertyID, const QVariant>>& properties, SIPropertyWriteFlags flags) {
    return writeProperties_(properties, flags);
}

QJsonObject SIDevice::jsonDescription(SIAccessLevel accessLevel, SIJsonFlags flags) const {
    QJsonObject description;

    description["model"] = model();
    description["id"] = id();
    if (flags.testFlag(SIJsonFlag::IncludePropertyInformation)) {
        QJsonArray props;
        for (const auto& property: properties()) {
            if (accessLevel >= property.accessLevel()) {
                props.append(property.jsonDescription(flags));
            }
        }
        description["properties"] = props;
        completeJsonDescription_(description, flags);
    }

    return description;
}

QVector<SIPropertyReadResult> SIDevice::readProperties_(const QVector<SIPropertyID>& ids) const {
    QVector<SIPropertyReadResult> results(ids.count());
    for (const auto& id: ids) {
        results.append(readProperty_(id));
    }
    return results;
}

QVector<SIPropertyWriteResult> SIDevice::writeProperties_(const QVector<const QPair<SIPropertyID, const QVariant>>& properties, SIPropertyWriteFlags flags) {
    QVector<SIPropertyWriteResult> results(properties.count());
    for (const auto& property: properties) {
        results.append(writeProperty_(property.first, property.second, flags));
    }
    return results;
}

void SIDevice::completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const {
    Q_UNUSED(object)
    Q_UNUSED(flags)
}
