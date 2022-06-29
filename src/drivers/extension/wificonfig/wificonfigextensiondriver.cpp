#include "wificonfigextensiondriver.h"
#include "wificonfigextension.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(WifiConfig, "driver.extension.WifiConfig", QtInfoMsg)

SIExtension* WifiConfigExtensionDriver::createExtensionInstance(const QVariantMap& parameters) {
    auto allowedUsers = parameters.value("allowedUsers").toStringList();
    if (allowedUsers.isEmpty()) {
        qCCritical(WifiConfig,) << "Missing required parameter \"allowedUsers\"";
        return nullptr;
    }
    for (auto& user: allowedUsers) {
        user = user.trimmed();
    }

    auto countryCode = parameters.value("countryCode").toString();
    if (countryCode.length() != 2) {
        qCCritical(WifiConfig,) << "Missing or invalid parameter \"countryCode\"";
        return nullptr;
    }

    auto accessPointInstall = parameters.value("accessPointInstall", "true").toBool();
    if (accessPointInstall && !WifiConfigExtension::isAPInterfacePresent()) {
        qCInfo(WifiConfig,) << "**********************************************************";
        qCInfo(WifiConfig,) << "* Installing WiFi AP mode packages and configuration ... *";
        qCInfo(WifiConfig,) << "* System will reboot after that.                         *";
        qCInfo(WifiConfig,) << "**********************************************************";
        WifiConfigExtension::AccessPointSettings initialAccessPointSettings {
            parameters.value("accessPointInitialEnabled").toBool(),
            1,
            parameters.value("accessPointInitialSSID", "openstuder").toString(),
            parameters.value("accessPointInitialPassKey", "openstuder").toString()
        };
        if (! WifiConfigExtension::installAccessPointRequirements(initialAccessPointSettings, countryCode)) {
            qCCritical(WifiConfig,) << "Failed to install AP mode packages and configuration";
        }
    }

    return new WifiConfigExtension(allowedUsers, countryCode);
}
