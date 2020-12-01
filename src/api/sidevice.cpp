#include "sidevice.h"
#include "sideviceaccess.h"
#include <QJsonArray>

struct SIDevice::Private_ {
    QString model;
    QString id;
    mutable SIJsonFlags cachedJsonFlags = SIJsonFlag::None;
    mutable QJsonObject cachedJsonDescription;
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


const QJsonObject& SIDevice::jsonDescription(SIJsonFlags flags) const {
    if (priv_->cachedJsonFlags != flags) {
        priv_->cachedJsonDescription = {};
    }
    priv_->cachedJsonFlags = flags;

    if (priv_->cachedJsonDescription.isEmpty()) {
        priv_->cachedJsonDescription["model"] = model();
        priv_->cachedJsonDescription["id"] = id();
        if (flags.testFlag(SIJsonFlag::IncludeDeviceDetails)) {
            QJsonArray props;
            for (const auto& property: properties()) {
                if (flags.testFlag(SIJsonFlag::IncludeExpertProperties) || !property.flags.testFlag(SIPropertyFlag::Expert)) {
                    props.append(QJsonObject {
                        {"id",          (int)property.id},
                        {"type",        to_string(property.type)},
                        {"readable",    property.flags.testFlag(SIPropertyFlag::Readable)},
                        {"writeable",   property.flags.testFlag(SIPropertyFlag::Writeable)},
                        {"description", property.description},
                        {"unit",        property.unit}
                    });
                }
            }
            priv_->cachedJsonDescription["properties"] = props;
            completeJsonDescription_(priv_->cachedJsonDescription, flags);
        }
    }
    return priv_->cachedJsonDescription;
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
