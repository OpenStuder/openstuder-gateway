#pragma once
#include <siaccesslevel.h>
#include <QSettings>
#include <memory>

class SISettings {
  public:
    ~SISettings() = default;

    inline QString driverSearchPaths() const {
        return valueWithDefault_("Gateway/driverSearchPaths", OPENSTUDER_GATEWAY_DEFAULT_DRIVER_LOCATION, gatewaySettings_.get()).toString();
    }

    inline int propertyPollInterval() const {
        return valueWithDefault_("Gateway/propertyPollInterval", 10000, gatewaySettings_.get()).toInt();
    }

    inline QString storageDriver() const {
        return valueWithDefault_("Storage/driver", "SQLite", gatewaySettings_.get()).toString();
    }

    inline QVariantMap storageOptions() const {
        return filteredChildSettings_("Storage", {"driver"}, gatewaySettings_.get());
    }

    inline bool webSocketEnabled() const {
        return valueWithDefault_("WebSocket/enabled", true, gatewaySettings_.get()).toBool();
    }

    inline quint16 webSocketPort() const {
        return valueWithDefault_("WebSocket/port", 1987, gatewaySettings_.get()).toUInt();
    }

    inline bool bluetoothEnabled() const {
        return valueWithDefault_("Bluetooth/enabled", false, gatewaySettings_.get()).toBool();
    }

    inline QString bluetoothName() const {
        return valueWithDefault_("Bluetooth/name", "StuderGW", gatewaySettings_.get()).toString();
    }

    inline bool authorizeEnabled() const {
        return valueWithDefault_("Authorize/enabled", false, gatewaySettings_.get()).toBool();
    }

    inline QString authorizeDriver() const {
        return valueWithDefault_("Authorize/driver", "Internal", gatewaySettings_.get()).toString();
    }

    SIAccessLevel authorizeGuestAccessLevel() const;

    inline QVariantMap authorizeOptions() const {
        return filteredChildSettings_("Authorize", {"enabled", "driver", "guestAccessLevel"}, gatewaySettings_.get());
    }

    inline QStringList deviceAccessConfigurationNames() const {
        return filteredChildGroups_({}, driverSettings_.get());
    }

    inline QString deviceAccessConfigurationDriver(const QString& deviceAccessConfigurationName) const {
        return requiredValue_(deviceAccessConfigurationName + "/driver", driverSettings_.get()).toString();
    };

    inline QVariantMap deviceAccessDriverOptions(const QString& deviceAccessConfigurationName) const {
        return filteredChildSettings_(deviceAccessConfigurationName, {"driver"}, driverSettings_.get());
    };

    inline QStringList extensionNames() const {
        if (extensionSettings_ != nullptr) {
            return filteredChildGroups_({}, extensionSettings_.get());
        } else {
            return {};
        }
    }

    inline QVariantMap extensionOptions(const QString& extensionName) const {
        if (extensionSettings_ != nullptr) {
            return filteredChildSettings_(extensionName, {}, extensionSettings_.get());
        } else {
            return {};
        }
    }

    static void loadFromLocation(const QString& location);

    static const SISettings& sharedSettings();

  private:
    explicit SISettings(QSettings* gateway = nullptr, QSettings* driver = nullptr, QSettings* extension = nullptr);

    static QVariant requiredValue_(const QString& key, const QSettings* settings);
    static QVariant valueWithDefault_(const QString& key, const QVariant& defaultValue, const QSettings* settings);
    static QVariant valueOfThese_(const QString& key, std::initializer_list<QVariant> validValues, const QVariant& defaultValue, const QSettings* settings);
    static QVariantMap filteredChildSettings_(const QString& group, const QStringList& exclude, QSettings* settings);
    static QStringList filteredChildGroups_(const QStringList& exclude, const QSettings* settings);

    std::shared_ptr<QSettings> gatewaySettings_;
    std::shared_ptr<QSettings> driverSettings_;
    std::shared_ptr<QSettings> extensionSettings_;

    static SISettings sharedSettings_;
};
