#include "wificonfigextension.h"
#include <QLoggingCategory>
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTemporaryFile>
#include <QDir>
#include <QEventLoop>
#include <QNetworkInterface>
#include <QSettings>

Q_DECLARE_LOGGING_CATEGORY(WifiConfig)

WifiConfigExtension::WifiConfigExtension(QStringList allowedUsers, QString country): SIExtension("WifiConfig", allowedUsers),
                                                                                     countryCode_(std::move(country)) {}

WifiConfigExtension::~WifiConfigExtension() = default;

WifiConfigExtension::Status WifiConfigExtension::status() {
    return {
        isClientInterfacePresent() && isClientEnabled(),
        isClientConnected(),
        clientSSID(),
        clientIPAddress(),
        isAPInterfacePresent() && isAPEnabled(),
        apSSID(),
        isWiredConnected(),
        wiredIPAddress()
    };
}

void WifiConfigExtension::scan(const ScanCallback& callback) {
    if (! isClientInterfacePresent()) {
        qCCritical(WifiConfig,) << "Error scanning for new WiFi networks: No WiFi client interface present";
        callback(SIExtensionStatus::Error, {});
    }

    auto* process = new QProcess;
    connect(process, qOverload<int,QProcess::ExitStatus>(&QProcess::finished), [callback, process](int exitCode, QProcess::ExitStatus exitStatus) {
        Q_UNUSED(exitStatus)
        if (exitCode == 0) {
            callback(SIExtensionStatus::Success, parseWifiScanOutput_(process->readAllStandardOutput()));
        } else {
            qCCritical(WifiConfig,) << "Unknown error scanning for new WiFi networks";
            callback(SIExtensionStatus::Error, {});
        }
        process->deleteLater();
    });
    process->start("iwlist", {"wlan0", "scan"});
}

SIExtensionStatus WifiConfigExtension::clientSetup(const ClientSettings& settings) {
    if (! isClientInterfacePresent()) {
        qCCritical(WifiConfig,) << "Error during client setup: No WiFi client interface present";
        return SIExtensionStatus::Error;
    }

    QFile configFile("/etc/wpa_supplicant/wpa_supplicant.conf");
    if (! configFile.open(QIODevice::ReadWrite)) {
        qCCritical(WifiConfig,) << "Error during client setup: Could not open WPA supplicant config file";
        return SIExtensionStatus::Error;
    }
    configFile.resize(0);

    QTextStream config(&configFile);
    config << "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev" << Qt::endl
           << "update_config=1" << Qt::endl << "country=" << countryCode_ << Qt::endl << Qt::endl;

    if (settings.enabled) {
        if (settings.passKey.isEmpty()) {
            config << "network={" << Qt::endl
            << "    ssid=\"" << settings.ssid << "\"" << Qt::endl
            << "    key_mgmt=NONE" << Qt::endl
            << "}" << Qt::endl;
        } else {
            auto result = execute_("wpa_passphrase", {settings.ssid, settings.passKey});
            if (result.exitCode != 0) {
                qCCritical(WifiConfig,) << "Error during client setup: Failed to generate WPA passkey";
                return SIExtensionStatus::Error;
            }

            config << result.stdOut;
        }
    }
    config.flush();
    configFile.close();

    if (execute_("wpa_cli", {"-i", "wlan0", "reconfigure"}).exitCode != 0) {
        qCCritical(WifiConfig,) << "Error during client setup: Failed to reload new configuration";
        return SIExtensionStatus::Error;
    }

    return SIExtensionStatus::Success;
}

SIExtensionStatus WifiConfigExtension::apSetup(const AccessPointSettings& settings, const QString& countryCode) {
    Q_UNUSED(settings);
    if (! isAPInterfacePresent()) {
        qCCritical(WifiConfig,) << "Error during AP setup: No WiFi AP interface present";
        return SIExtensionStatus::Error;
    }

    if (settings.enabled) {
        if (execute_("systemctl", {"stop", "hostapd"}).exitCode != 0) {
            qCCritical(WifiConfig,) << "Error during AP setup: Failed to stop hostapd";
            return SIExtensionStatus::Error;
        }

        QString conf = QString(R"(
channel=%1
ssid=%2
wpa_passphrase=%3
country_code=%4
interface=uap0
# Use the 2.4GHz band (I think you can use in ag mode to get the 5GHz band as well, but I have not tested this yet)
hw_mode=g
# Accept all MAC addresses
macaddr_acl=0
# Use WPA authentication
auth_algs=1
# Require clients to know the network name
ignore_broadcast_ssid=0
# Use WPA2
wpa=2
# Use a pre-shared key
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
#driver=nl80211
# I commented out the lines below in my implementation, but I kept them here for reference.
# Enable WMM
#wmm_enabled=1
# Enable 40MHz channels with 20ns guard interval
#ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40])")
            .arg(settings.channel).arg(settings.ssid).arg(settings.passKey).arg(countryCode);

        QFile hostapdSettings("/etc/hostapd/hostapd.conf");
        if (! hostapdSettings.open(QIODevice::ReadWrite)) {
            qCCritical(WifiConfig,) << "Error during AP setup: Could not open hostapd config file";
            return SIExtensionStatus::Error;
        }

        hostapdSettings.resize(0);
        hostapdSettings.write(conf.toLocal8Bit());
        hostapdSettings.close();

        if (execute_("systemctl", {"enable", "hostapd"}).exitCode != 0) {
            qCCritical(WifiConfig,) << "Error during AP setup: Failed to enable hostapd";
            return SIExtensionStatus::Error;
        }
        if (execute_("systemctl", {"start", "hostapd"}).exitCode != 0) {
            qCCritical(WifiConfig,) << "Error during AP setup: Failed to start hostapd";
            return SIExtensionStatus::Error;
        }
    } else {
        if (execute_("systemctl", {"stop", "hostapd"}).exitCode != 0) {
            qCCritical(WifiConfig,) << "Error during AP setup: Failed to stop hostapd";
            return SIExtensionStatus::Error;
        }
        if (execute_("systemctl", {"disable", "hostapd"}).exitCode != 0) {
            qCCritical(WifiConfig,) << "Error during AP setup: Failed to disable hostapd";
            return SIExtensionStatus::Error;
        }
    }

    return SIExtensionStatus::Success;
}

bool WifiConfigExtension::isClientInterfacePresent() {
    return QNetworkInterface::interfaceIndexFromName("wlan0") != 0;
}

bool WifiConfigExtension::isClientEnabled() {
    auto wlan0 = QNetworkInterface::interfaceFromName("wlan0");
    return wlan0.isValid() && wlan0.flags().testFlag(QNetworkInterface::IsRunning);
}

bool WifiConfigExtension::isClientConnected() {
    auto wlan0 = QNetworkInterface::interfaceFromName("wlan0");
    return wlan0.isValid() && wlan0.flags().testFlag(QNetworkInterface::IsRunning) && wlan0.addressEntries().count() > 1;
}

QString WifiConfigExtension::clientSSID() {
    auto result = execute_("iwgetid",  {"-r", "wlan0"});
    if (result.exitCode == 0) {
        return result.stdOut.trimmed();
    } else {
        return {};
    }
}

QString WifiConfigExtension::clientIPAddress() {
    auto wlan0 = QNetworkInterface::interfaceFromName("wlan0");
    auto addresses = wlan0.addressEntries();
    for (const auto& address: addresses) {
        if (address.ip().isGlobal() && address.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            return address.ip().toString();
        }
    }
    for (const auto& address: addresses) {
        if (address.ip().isGlobal() && address.ip().protocol() == QAbstractSocket::IPv6Protocol) {
            return address.ip().toString();
        }
    }
    return {};
}

bool WifiConfigExtension::isAPInterfacePresent() {
    return QNetworkInterface::interfaceIndexFromName("uap0") != 0;
}

bool WifiConfigExtension::isAPEnabled() {
    QNetworkInterface uap0 = QNetworkInterface::interfaceFromName("uap0");
    return uap0.isValid() && uap0.flags().testFlag(QNetworkInterface::IsRunning);
}

QString WifiConfigExtension::apSSID() {
    auto result = execute_("grep", {"^ssid=", "/etc/hostapd/hostapd.conf"});
    if (result.exitCode == 0) {
        return result.stdOut.mid(5).trimmed();
    } else {
        return {};
    }
}

bool WifiConfigExtension::isWiredConnected() {
    QNetworkInterface eth0 = QNetworkInterface::interfaceFromName("eth0");
    return eth0.isValid() && eth0.flags().testFlag(QNetworkInterface::IsRunning);
}

QString WifiConfigExtension::wiredIPAddress() {
    QNetworkInterface eth0 = QNetworkInterface::interfaceFromName("eth0");
    auto addresses = eth0.addressEntries();
    for (const auto& address: addresses) {
        if (address.ip().isGlobal() && address.ip().protocol() == QAbstractSocket::IPv4Protocol) {
            return address.ip().toString();
        }
    }
    for (const auto& address: addresses) {
        if (address.ip().isGlobal() && address.ip().protocol() == QAbstractSocket::IPv6Protocol) {
            return address.ip().toString();
        }
    }
    return {};
}

bool WifiConfigExtension::installAccessPointRequirements(const AccessPointSettings& settings, const QString& countryCode) {
    auto installScript = prepareAPInstallScript_();
    if (installScript.isEmpty()) {
        qCCritical(WifiConfig,) << "Error during AP install: Failed to generate install script";
        return false;
    }

    QProcess::execute(installScript, {
        "--install",
        QString("--ap-ssid=\"%1\"").arg(settings.ssid),
        QString("--ap-password=\"%1\"").arg(settings.passKey),
        QString("--ap-country-code=\"%4\"").arg(countryCode)
    });

    return true;
}

void WifiConfigExtension::uninstallAccessPointRequirements() {
    auto installScript = prepareAPInstallScript_();
    if (installScript.isEmpty()) {
        qCCritical(WifiConfig,) << "Error during AP uninstall: Failed to generate install script";
        return;
    }

    QProcess::execute(installScript, {"--clean"});
}

WifiConfigExtension::ExecutionResult WifiConfigExtension::execute_(const QString& command, const QStringList& arguments) {
    QProcess process;
    QEventLoop loop;
    connect(&process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus status) {
        if (status == QProcess::CrashExit) {
            qCCritical(WifiConfig,) << "command" << command << arguments << "crashed";
        }
        if (exitCode != 0) {
            qCWarning(WifiConfig,) << "command" << command << arguments << "exited with exit code" << exitCode;
        }
        loop.quit();
    });
    process.start(command, arguments);
    loop.exec();
    return {process.exitCode(), process.readAllStandardOutput()};
}

WifiConfigExtension::ScanResults WifiConfigExtension::parseWifiScanOutput_(const QByteArray& output) {
    static QRegularExpression cellRegex("^(?:\\s|\\t)*Cell (\\d)+ - Address: [\\d,A-F,:]+$");
    static QRegularExpression ssidRegex("^(?:\\s|\\t)+ESSID:\"(.*)\"$");
    static QRegularExpression signalLevelRegex("^.*Signal level=(.*) dBm\\s*$");
    static QRegularExpression encryptionRegex("^(?:\\s|\\t)+Encryption key:(.*)$");

    ScanResults list;
    QTextStream outStream(output);

    QString line;
    while (outStream.readLineInto(&line)) {
        auto cellMatch = cellRegex.match(line);
        if (cellMatch.hasMatch()) {
            list.append(ScanResultEntry());
            continue;
        }

        auto ssidMatch = ssidRegex.match(line);
        if (ssidMatch.hasMatch() && ! list.isEmpty()) {
            list.back().ssid = ssidMatch.captured(1);
            continue;
        }

        auto signalLevelMatch = signalLevelRegex.match(line);
        if (signalLevelMatch.hasMatch() && ! list.isEmpty()) {
            list.back().signalLevel = signalLevelMatch.captured(1).toInt();
            continue;
        }

        auto encryptionMatch = encryptionRegex.match(line);
        if (encryptionMatch.hasMatch() && ! list.isEmpty()) {
            list.back().encrypted = encryptionMatch.captured(1) == "on";
            continue;
        }
    }

    return list;
}

QByteArray WifiConfigExtension::encodeScanResultsAsJson_(const WifiConfigExtension::ScanResults& scanResults) {
    QJsonArray results;

    for (const auto& scanResult: scanResults) {
        results.append(QJsonObject {
            {"ssid",      scanResult.ssid},
            {"signal",    scanResult.signalLevel},
            {"encrypted", scanResult.encrypted}
        });
    }

    return QJsonDocument(results).toJson(QJsonDocument::Compact);
}

QVector<QVariant> WifiConfigExtension::encodeScanResultsAsList_(const WifiConfigExtension::ScanResults& scanResults) {
    QVector<QVariant> results;

    for (const auto& scanResult: scanResults) {
        results << scanResult.ssid;
    }

    return results;
}

QString WifiConfigExtension::prepareAPInstallScript_() {
    QFile installScriptSource(":ap-setup.sh");
    if (! installScriptSource.open(QIODevice::ReadOnly)) {
        return {};
    }

    QTemporaryFile installScript(QDir::tempPath() + QDir::separator() + "openstuder-ap-setup-XXXXXX.sh");
    installScript.setAutoRemove(false);
    if (! installScript.open()) {
        return {};
    }

    installScript.write(installScriptSource.readAll());
    installScript.close();

    if (execute_("chmod", {"+x", installScript.fileName()}).exitCode != 0) {
        QFile::remove(installScript.fileName());
        return {};
    }

    return installScript.fileName();
}

QStringList& WifiConfigExtension::commands_() const {
    static QStringList commands_ {"status", "scan", "cliconf", "apconf"};
    return commands_;
}

SIExtensionWebSocketResult* WifiConfigExtension::runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
    Q_UNUSED(context);
    Q_UNUSED(body);

    if (command == "status") {
        if (! validateWebSocketHeaders(headers, {})) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        auto status_ = status();
        return new SIExtensionWebSocketResult(SIExtensionStatus::Success, {
            {"client_enabled",    status_.clientEnabled ? "true" : "false"},
            {"client_connected", status_.clientConnected ? "true" : "false"},
            {"client_ssid", status_.clientSSID},
            {"client_ip",        status_.clientIPAddress},
            {"ap_enabled",        status_.accessPointEnabled ? "true" : "false"},
            {"ap_ssid", status_.accessPointSSID},
            {"wired_connected",     status_.wiredConnected ? "true" : "false"},
            {"wired_ip", status_.wiredIPAddress}
        });
    } else if (command == "scan") {
        if (! validateWebSocketHeaders(headers, {})) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        auto* result = new SIExtensionWebSocketResult();
        scan([result](SIExtensionStatus status, const ScanResults& scanResults) {
            result->complete(status, {}, encodeScanResultsAsJson_(scanResults));
        });
        return result;
    } else if (command == "cliconf") {
        if (! validateWebSocketHeaders(headers, {"enabled"}, {"ssid", "passkey"})) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        auto enabledString = headers["enabled"];
        auto enabled = enabledString.toLower() == "true";
        auto ssid = headers["ssid"];
        auto passKey = headers["passkey"];

        if (enabledString.isEmpty() || (enabled && (ssid.isEmpty() || (!passKey.isEmpty() && passKey.length() < 8)))) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        return SIExtensionWebSocketResult::fromStatus(clientSetup({
                                                                      enabled,
                                                                      ssid,
                                                                      passKey
                                                                  }));
    } else if (command == "apconf") {
        if (! validateWebSocketHeaders(headers, {"enabled"}, {"channel", "ssid", "passkey"})) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        auto enabledString = headers["enabled"];
        auto enabled = enabledString.toLower() == "true";
        auto channel = headers.value("channel", "1").toInt();
        auto ssid = headers["ssid"];
        auto passKey = headers["passkey"];

        if (enabledString.isEmpty() || (enabled && (ssid.isEmpty() || passKey.length() < 8 || channel < 1 || channel > 13))) {
            return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::InvalidHeaders);
        }

        return SIExtensionWebSocketResult::fromStatus(apSetup({
                                                                  enabled,
                                                                  channel,
                                                                  ssid,
                                                                  passKey
                                                              }, countryCode_));
    } else {
        return SIExtensionWebSocketResult::fromStatus(SIExtensionStatus::UnsupportedCommand);
    }
}

bool WifiConfigExtension::bluetoothSupported_() const {
    return true;
}

SIExtensionBluetoothResult* WifiConfigExtension::runCommand_(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters) {
    Q_UNUSED(context);

    if (command == "status") {
        if (! validateBluetoothParameters(parameters, {})) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        auto status_ = status();
        return new SIExtensionBluetoothResult(SIExtensionStatus::Success, {
            status_.clientEnabled,
            status_.clientConnected,
            status_.clientIPAddress,
            status_.accessPointEnabled,
            status_.wiredConnected,
            status_.wiredIPAddress
        });
    } else if (command == "scan") {
        if (! validateBluetoothParameters(parameters, {})) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        auto* result = new SIExtensionBluetoothResult();
        scan([result](SIExtensionStatus status, const ScanResults& scanResults) {
            result->complete(status, encodeScanResultsAsList_(scanResults));
        });
        return result;
    } else if (command == "cliconf") {
        if (! validateBluetoothParameters(parameters, {{QVariant::Bool}}, true)) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        auto enabled = parameters[0].toBool();
        if (enabled && ! validateBluetoothParameters(parameters, {{QVariant::Bool},
                                                                  {QVariant::String},
                                                                  {QVariant::String}})) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        if (enabled) {
            auto ssid = parameters[1].toString();
            auto passKey = parameters[2].toString();
            if (!passKey.isEmpty() && passKey.length() < 8) {
                return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
            }

            return SIExtensionBluetoothResult::fromStatus(clientSetup({enabled, ssid, passKey}));
        } else {
            return SIExtensionBluetoothResult::fromStatus(clientSetup({false, "", ""}));
        }

    } else if (command == "apconf") {
        if (! validateBluetoothParameters(parameters, {{QVariant::Bool}}, true)) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        auto enabled = parameters[0].toBool();
        if (enabled && ! validateBluetoothParameters(parameters, {{QVariant::Bool},
                                                                  {QVariant::Int},
                                                                  {QVariant::String},
                                                                  {QVariant::String}})) {
            return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
        }

        if (enabled) {
            auto channel = parameters[1].toInt();
            auto ssid = parameters[2].toString();
            auto passKey = parameters[3].toString();
            if (passKey.length() < 8 || channel < 1 || channel > 13) {
                return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::InvalidParameters);
            }
            return SIExtensionBluetoothResult::fromStatus(apSetup({enabled, channel, ssid, passKey}, countryCode_));
        } else {
            return SIExtensionBluetoothResult::fromStatus(apSetup({false, 1, "", ""}, countryCode_));
        }
    } else {
        return SIExtensionBluetoothResult::fromStatus(SIExtensionStatus::UnsupportedCommand);
    }
}
