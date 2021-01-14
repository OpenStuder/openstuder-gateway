#pragma once
#include <QSettings>
#include <memory>

class SISettings {
  public:
    ~SISettings() = default;

    inline QString driverSearchPaths() const {
        return valueWithDefault_("Gateway/driverSearchPaths", "/var/lib/sigateway/drivers").toString();
    }

    inline int propertyPollInterval() const {
        return valueWithDefault_("Gateway/propertyPollInterval", 60000).toInt();
    }

    inline bool webSocketEnabled() const {
        return valueWithDefault_("WebSocket/enabled", true).toBool();
    }

    inline quint16 webSocketPort() const {
        return valueWithDefault_("WebSocket/port", 1987).toUInt();
    }

    inline bool bluetoothEnabled() const {
        return valueWithDefault_("Bluetooth/enabled", false).toBool();
    }

    inline QString bluetoothName() const {
        return valueWithDefault_("Bluetooth/name", "SIGateway").toString();
    }

    inline bool securityAllowGuest() const {
        return valueWithDefault_("Security/allowGuest", false).toBool();
    }

    inline QString storageDriver() const {
        return valueWithDefault_("driver", "SQLite").toString();
    }

    inline QVariantMap storageOptions() const {
        return filteredChildSettings_("Storage", {"driver"});
    }

    inline QStringList deviceAccessConfigurationNames() const {
        return filteredChildGroups_({"Gateway", "Storage", "WebSocket", "Bluetooth", "Security"});
    }

    inline QString deviceAccessConfigurationDriver(const QString& deviceAccessConfigurationName) const {
        return requiredValue_(deviceAccessConfigurationName + "/driver").toString();
    };

    inline QVariantMap deviceAccessDriverOptions(const QString& deviceAccessConfigurationName) const {
        return filteredChildSettings_(deviceAccessConfigurationName, {"driver"});
    };

    static void loadFromLocation(const QString& location);

    static const SISettings& sharedSettings();

  private:
    explicit SISettings(QSettings* settings = nullptr);

    QVariant requiredValue_(const QString& key) const;
    QVariant valueWithDefault_(const QString& key, const QVariant& defaultValue) const;
    QVariant valueOfThese_(const QString& key, std::initializer_list<QVariant> validValues, const QVariant& defaultValue) const;
    QVariantMap filteredChildSettings_(const QString& group, const QStringList& exclude) const;
    QStringList filteredChildGroups_(const QStringList& exclude) const;

    std::shared_ptr<QSettings> settings_;
    static SISettings sharedSettings_;
};
