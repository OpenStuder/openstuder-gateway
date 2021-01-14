#include "sisettings.h"
#include <stdexcept>
#include <QFile>

void SISettings::loadFromLocation(const QString& location) {
    if (!QFile::exists(location)) {
        throw std::runtime_error(QString("Configuration file %1 does not exist").arg(location).toStdString());
    }
    auto settings = new QSettings(location, QSettings::IniFormat);
    if (settings->status() != QSettings::NoError) {
        throw std::runtime_error(QString("Configuration file %1 is malformed").arg(location).toStdString());
    }
    sharedSettings_ = SISettings(settings);
}

const SISettings& SISettings::sharedSettings() {
    return sharedSettings_;
}

SISettings::SISettings(QSettings* settings): settings_(settings) {}

QVariant SISettings::requiredValue_(const QString& key) const {
    if (settings_ == nullptr || !settings_->contains(key)) {
        throw std::runtime_error(QString("Missing mandatory configuration setting \"%1\"").arg(key).toStdString());
    }
    return settings_->value(key);
}

QVariant SISettings::valueWithDefault_(const QString& key, const QVariant& defaultValue) const {
    return settings_ != nullptr ? settings_->value(key, defaultValue) : defaultValue;
}

QVariant SISettings::valueOfThese_(const QString& key, std::initializer_list<QVariant> validValues, const QVariant& defaultValue) const {
    auto value = valueWithDefault_(key, defaultValue);
    for (const auto validValue: validValues) {
        if (value == validValue) {
            return value;
        }
    }
    throw std::runtime_error(QString("Invalid configuration value \"%1\" for key \"%2\"").arg(value.toString()).arg(key).toStdString());
}

QVariantMap SISettings::filteredChildSettings_(const QString& group, const QStringList& exclude) const {
    QVariantMap filtered;
    if (settings_ != nullptr && settings_->childGroups().contains(group)) {
        settings_->beginGroup(group);

        for (const auto& key: settings_->childKeys()) {
            if (!exclude.contains(key)) {
                filtered[key] = settings_->value(key);
            }
        }
        settings_->endGroup();
    }
    return filtered;
}

QStringList SISettings::filteredChildGroups_(const QStringList& exclude) const {
    QStringList filtered;
    if (settings_ != nullptr) {
        for (const auto& group: settings_->childGroups()) {
            if (!exclude.contains(group)) {
                filtered << group;
            }
        }
    }
    return filtered;
}

SISettings SISettings::sharedSettings_ {};