#include "siproperty.h"
#include <utility>

static const QString emptyString_ = ""; // NOLINT(cert-err58-cpp)

struct SIProperty::Private_ {
    Private_(SIPropertyID id, SIPropertyType type, SIAccessLevel accessLevel, SIPropertyFlags flags, QString description, QString unit = "")
        : id(id), type(type), accessLevel(accessLevel), flags(flags), description(std::move(description)), unit(std::move(unit)) {}

    SIPropertyID id;
    SIPropertyType type;
    SIAccessLevel accessLevel;
    SIPropertyFlags flags;
    QString description;
    QString unit;
    QJsonObject enumValues;
};

SIProperty::SIProperty() = default;

SIProperty::SIProperty(SIPropertyID id, SIPropertyType type, SIAccessLevel accessLevel, SIPropertyFlags flags, const QString& description, const QString& unit):
    private_(new Private_(id, type, accessLevel, flags, description, unit)) {}

SIProperty::SIProperty(SIPropertyID id, SIPropertyType type, SIAccessLevel accessLevel, SIPropertyFlags flags, const QString& description, const std::initializer_list<std::pair<int, const char*>>& enumValues):
    private_(new Private_(id, type, accessLevel, flags, description)) {
    for (const auto& enumValue: enumValues) {
        private_->enumValues[QString::number(enumValue.first)] = enumValue.second;
    }
}

SIPropertyID SIProperty::id() const {
    if (private_) {
        return private_->id;
    } else {
        return 0;
    }
}

SIPropertyType SIProperty::type() const {
    if (private_) {
        return private_->type;
    } else {
        return SIPropertyType::Invalid;
    }
}

SIAccessLevel SIProperty::accessLevel() const {
    if (private_) {
        return private_->accessLevel;
    } else {
        return SIAccessLevel::None;
    }
}

SIPropertyFlags SIProperty::flags() const {
    if (private_) {
        return private_->flags;
    } else {
        return SIPropertyFlag::None;
    }
}

bool SIProperty::isFlagSet(SIPropertyFlag flag) const {
    if (private_) {
        return private_->flags.testFlag(flag);
    } else {
        return false;
    }
}

const QString& SIProperty::description() const {
    if (private_) {
        return private_->description;
    } else {
        return emptyString_;
    }
}

const QString& SIProperty::unit() const {
    if (private_) {
        return private_->unit;
    } else {
        return emptyString_;
    }
}

QJsonObject SIProperty::jsonDescription(SIDescriptionFlags flags) const {
    Q_UNUSED(flags)

    if (private_) {
        QJsonObject description {
            {"id",          (int)private_->id},
            {"type",        to_string(private_->type)},
            {"readable",    private_->flags.testFlag(SIPropertyFlag::Readable)},
            {"writeable",   private_->flags.testFlag(SIPropertyFlag::Writeable)},
            {"description", private_->description}
        };
        if (private_->type == SIPropertyType::Enum) {
            description["values"] = private_->enumValues;
        } else {
            description["unit"] = private_->unit;
        }
        return description;
    } else {
        return {};
    }
}
