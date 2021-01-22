#pragma once
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
        return valueWithDefault_("Bluetooth/name", "SIGateway", gatewaySettings_.get()).toString();
    }

    inline bool securityEnabled() const {
        return valueWithDefault_("Security/enabled", false, gatewaySettings_.get()).toBool();
    }

    inline bool securityAllowGuest() const {
        return valueWithDefault_("Security/allowGuest", false, gatewaySettings_.get()).toBool();
    }

    inline QString storageDriver() const {
        return valueWithDefault_("Storage/driver", "SQLite", gatewaySettings_.get()).toString();
    }

    inline QVariantMap storageOptions() const {
        return filteredChildSettings_("Storage", {"driver"}, gatewaySettings_.get());
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

    static void loadFromLocation(const QString& location);

    static const SISettings& sharedSettings();

  private:
    explicit SISettings(QSettings* gateway = nullptr, QSettings* driver = nullptr);

    QVariant requiredValue_(const QString& key, const QSettings* settings) const;
    QVariant valueWithDefault_(const QString& key, const QVariant& defaultValue, const QSettings* settings) const;
    QVariant valueOfThese_(const QString& key, std::initializer_list<QVariant> validValues, const QVariant& defaultValue, const QSettings* settings) const;
    QVariantMap filteredChildSettings_(const QString& group, const QStringList& exclude, QSettings* settings) const;
    QStringList filteredChildGroups_(const QStringList& exclude, const QSettings* settings) const;

    std::shared_ptr<QSettings> gatewaySettings_;
    std::shared_ptr<QSettings> driverSettings_;
    static SISettings sharedSettings_;
};
