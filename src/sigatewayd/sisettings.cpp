#include "sisettings.h"
#include <stdexcept>
#include <QFile>

SIAccessLevel SISettings::authorizeGuestAccessLevel() const {
    auto value = valueOfThese_("Authorize/guestAccessLevel", {"None", "Basic", "Installer", "Expert", "QSP"}, "None", gatewaySettings_.get());
    if (value == "None") return SIAccessLevel::None;
    else if (value == "Basic") return SIAccessLevel::Basic;
    else if (value == "Installer") return SIAccessLevel::Installer;
    else if (value == "Expert") return SIAccessLevel::Expert;
    else if (value == "QSP") return SIAccessLevel::QualifiedServicePersonnel;
    return SIAccessLevel::None;
}

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

    QSettings* extensionSettings = nullptr;
    auto extensionSettingsFile = location + "/extensions.conf";
    if (QFile::exists(extensionSettingsFile)) {
        extensionSettings = new QSettings(extensionSettingsFile, QSettings::IniFormat);
        if (driverSettings->status() != QSettings::NoError) {
            delete extensionSettings;
            extensionSettings = new QSettings();
        }
    }

    sharedSettings_ = SISettings(settings, driverSettings, extensionSettings);
}

const SISettings& SISettings::sharedSettings() {
    return sharedSettings_;
}

SISettings::SISettings(QSettings* gateway, QSettings* driver, QSettings* extension): gatewaySettings_(gateway), driverSettings_(driver), extensionSettings_(extension) {}

QVariant SISettings::requiredValue_(const QString& key, const QSettings* settings) {
    if (settings == nullptr || !settings->contains(key)) {
        throw std::runtime_error(QString("Missing mandatory configuration setting \"%1\"").arg(key).toStdString());
    }
    return settings->value(key);
}

QVariant SISettings::valueWithDefault_(const QString& key, const QVariant& defaultValue, const QSettings* settings) {
    return settings != nullptr ? settings->value(key, defaultValue) : defaultValue;
}

QVariant SISettings::valueOfThese_(const QString& key, std::initializer_list<QVariant> validValues, const QVariant& defaultValue, const QSettings* settings) {
    auto value = valueWithDefault_(key, defaultValue, settings);
    for (const auto& validValue: validValues) {
        if (value == validValue) {
            return value;
        }
    }
    throw std::runtime_error(QString(R"(Invalid configuration value "%1" for key "%2")").arg(value.toString()).arg(key).toStdString());
}

QVariantMap SISettings::filteredChildSettings_(const QString& group, const QStringList& exclude, QSettings* settings) {
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

QStringList SISettings::filteredChildGroups_(const QStringList& exclude, const QSettings* settings) {
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

SISettings SISettings::sharedSettings_ {}; // NOLINT(cert-err58-cpp)