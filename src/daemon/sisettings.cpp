#include "sisettings.h"
#include <stdexcept>
#include <QFile>

void SISettings::loadFromLocation(const QString& location) {
    auto gatewaySettingsFile = location + "/gateway.conf";
    if (!QFile::exists(gatewaySettingsFile)) {
        throw std::runtime_error(QString("Configuration file %1 does not exist").arg(gatewaySettingsFile).toStdString());
    }
    auto settings = new QSettings(gatewaySettingsFile, QSettings::IniFormat);
    if (settings->status() != QSettings::NoError) {
        throw std::runtime_error(QString("Configuration file %1 is malformed").arg(gatewaySettingsFile).toStdString());
    }
    auto driverSettingsFile = location + "/drivers.conf";
    if (!QFile::exists(driverSettingsFile)) {
        throw std::runtime_error(QString("Driver configuration file %1 does not exist").arg(driverSettingsFile).toStdString());
    }
    auto driverSettings = new QSettings(driverSettingsFile, QSettings::IniFormat);
    if (driverSettings->status() != QSettings::NoError) {
        throw std::runtime_error(QString("Driver configuration file %1 is malformed").arg(driverSettingsFile).toStdString());
    }
    sharedSettings_ = SISettings(settings, driverSettings);
}

const SISettings& SISettings::sharedSettings() {
    return sharedSettings_;
}

SISettings::SISettings(QSettings* gateway, QSettings* driver): gatewaySettings_(gateway), driverSettings_(driver) {}

QVariant SISettings::requiredValue_(const QString& key, const QSettings* settings) const {
    if (settings == nullptr || !settings->contains(key)) {
        throw std::runtime_error(QString("Missing mandatory configuration setting \"%1\"").arg(key).toStdString());
    }
    return settings->value(key);
}

QVariant SISettings::valueWithDefault_(const QString& key, const QVariant& defaultValue, const QSettings* settings) const {
    return settings != nullptr ? settings->value(key, defaultValue) : defaultValue;
}

QVariant SISettings::valueOfThese_(const QString& key, std::initializer_list<QVariant> validValues, const QVariant& defaultValue, const QSettings* settings) const {
    auto value = valueWithDefault_(key, defaultValue, settings);
    for (const auto& validValue: validValues) {
        if (value == validValue) {
            return value;
        }
    }
    throw std::runtime_error(QString("Invalid configuration value \"%1\" for key \"%2\"").arg(value.toString()).arg(key).toStdString());
}

QVariantMap SISettings::filteredChildSettings_(const QString& group, const QStringList& exclude, QSettings* settings) const {
    QVariantMap filtered;
    if (settings != nullptr && settings->childGroups().contains(group)) {
        settings->beginGroup(group);

        for (const auto& key: settings->childKeys()) {
            if (!exclude.contains(key)) {
                filtered[key] = settings->value(key);
            }
        }
        settings->endGroup();
    }
    return filtered;
}

QStringList SISettings::filteredChildGroups_(const QStringList& exclude, const QSettings* settings) const {
    QStringList filtered;
    if (settings != nullptr) {
        for (const auto& group: settings->childGroups()) {
            if (!exclude.contains(group)) {
                filtered << group;
            }
        }
    }
    return filtered;
}

SISettings SISettings::sharedSettings_ {};