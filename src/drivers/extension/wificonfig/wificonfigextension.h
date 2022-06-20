#pragma once
#include <siextension.h>
#include <siaccesslevel.h>
#include <functional>

class WifiConfigExtension: public SIExtension {
  Q_OBJECT

  public:
    WifiConfigExtension(QStringList allowedUsers, QString country);
    ~WifiConfigExtension() override;

    struct Status {
        bool clientEnabled;
        bool clientConnected;
        QString clientIPAddress;
        bool accessPointEnabled;
    };
    static Status status();

    struct ScanResultEntry {
        QString ssid;
        int signalLevel;
        bool encrypted;
    };
    using ScanResults = QVector<ScanResultEntry>;
    using ScanCallback = std::function<void(SIExtensionStatus status, const ScanResults&)>;
    static void scan(const ScanCallback& callback);

    struct ClientSettings  {
        bool enabled;
        QString ssid;
        QString passKey;
    };
    SIExtensionStatus clientSetup(const ClientSettings& settings);

    struct AccessPointSettings {
        bool enabled;
        int channel;
        QString ssid;
        QString passKey;
    };
    SIExtensionStatus apSetup(const AccessPointSettings& settings, const QString& countryCode);

    static bool isClientInterfacePresent();
    static bool isClientEnabled();
    static bool isClientConnected();
    static QString clientIPAddress();

    static bool isAPInterfacePresent();
    static bool isAPEnabled();

    static bool installAccessPointRequirements(const AccessPointSettings& settings, const QString& countryCode);
    static void uninstallAccessPointRequirements();

  private:
    struct ExecutionResult {
        int exitCode;
        QByteArray stdOut;
    };
    static ExecutionResult execute_(const QString& command, const QStringList& arguments = {});

    static ScanResults parseWifiScanOutput_(const QByteArray& output);
    static QByteArray encodeScanResults_(const ScanResults& scanResults);

    static QString prepareAPInstallScript_();

    QStringList& commands_() const override;
    SIExtensionWebSocketResult* runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) override;

    bool bluetoothSupported_() const override;
    SIExtensionBluetoothResult* runCommand_(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters) override;

    QStringList allowedUsers_;
    QString country_;
};
