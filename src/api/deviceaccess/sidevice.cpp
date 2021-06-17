#include "sidevice.h"
#include "sideviceaccess.h"
#include <QJsonArray>

struct SIDevice::Private_ {
    QString model;
    QString id;
    bool isVirtual;
};

SIDevice::SIDevice(const QString& model, const QString& id,  bool isVirtual): private_(new Private_) {
    private_->model = model;
    private_->id = id;
    private_->isVirtual = isVirtual;
}

SIDevice::~SIDevice() = default;

const QString& SIDevice::model() const {
    return private_->model;
}

const QString& SIDevice::id() const {
    return private_->id;
}

bool SIDevice::isVirtual() const {
    return private_->isVirtual;
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

QJsonObject SIDevice::jsonDescription(SIAccessLevel accessLevel, SIDescriptionFlags flags) const {
    QJsonObject description;

    description["model"] = model();
    description["id"] = id();
    description["virtual"] = isVirtual();
    if (flags.testFlag(SIDescriptionFlag::IncludePropertyInformation)) {
        QJsonArray props;
        for (const auto& property: properties()) {
            if (accessLevel >= property.accessLevel()) {
                props.append(property.jsonDescription(flags));
            }
        }
        description["properties"] = props;
    }

    completeJsonDescription_(description, flags);

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

void SIDevice::completeJsonDescription_(QJsonObject& object, SIDescriptionFlags flags) const {
    Q_UNUSED(object)
    Q_UNUSED(flags)
}
