#include "sidaemon.h"
#include "deviceaccess/sideviceaccessmanager.h"
#include "deviceaccess/sisequentialpropertymanager.h"
#include "websocket/siwebsocketmanager.h"
#include "bluetooth/sibluetoothmanager.h"
#include "sisettings.h"
#include "datalog/sidatalogconfiguration.h"
#include "datalog/sidatalogmanager.h"
#include <sistoragedriver.h>
#include <sidevice.h>
#include <sideviceaccess.h>
#include <sideviceaccessregistry.h>
#include <siuserauthorizedriver.h>
#include <sitextfileusermanagement.h>
#include <QCommandLineParser>
#include <QFile>
#include <QLoggingCategory>
#include <QElapsedTimer>
#include <memory>


using namespace std;

Q_LOGGING_CATEGORY(DAEMON, "daemon", QtInfoMsg)

SIDaemon::SIDaemon(int& argc, char** argv): QCoreApplication(argc, argv) {}

SIDaemon::~SIDaemon() = default;

bool SIDaemon::initialize() {
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setOrganizationName("openstuder");
    QCoreApplication::setApplicationName("sigatewayd");
    QCoreApplication::setApplicationVersion(OPENSTUDER_GATEWAY_VERSION);

    // Parse command line options.
    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption configurationFileOption {{"c", "config"}, "Use configuration location <dir>", "dir"};
    configurationFileOption.setDefaultValue(OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION);
    parser.addOption(configurationFileOption);
    QCommandLineOption verboseOption {{"v", "verbose"}, "Verbose mode. Prints out more information."};
    parser.addOption(verboseOption);
    QCommandLineOption silentOption {{"s", "silent"}, "Silent mode. Prints out less information."};
    parser.addOption(silentOption);
    parser.process(*this);

    // Setup logging.
    qSetMessagePattern("%{time}: %{category} [%{type}] %{message}");
    if (parser.isSet(verboseOption)) {
        QLoggingCategory::setFilterRules("*.debug=true");
    } else {
        if (parser.isSet(silentOption)) {
            QLoggingCategory::setFilterRules("*.info=false");
        }
    }

    // Load configuration file.
    auto configurationFileLocation = parser.value(configurationFileOption);
    try {
        SISettings::loadFromLocation(configurationFileLocation);
    } catch (std::runtime_error& error) {
        qCCritical(DAEMON,) << error.what();
        return false;
    }
    auto& settings = SISettings::sharedSettings();
    qCInfo(DAEMON,) << "Using configuration directory" << configurationFileLocation;

    // Load datalog configuration.
    QFile dataLogConfigurationFile(configurationFileLocation + "/datalog.conf");
    SIDataLogConfiguration dataLogConfiguration;
    try {
        dataLogConfiguration = SIDataLogConfiguration::parse(dataLogConfigurationFile);
    } catch (std::runtime_error& error) {
        qCCritical(DAEMON,) << "Error parsing datalog configuration file" << dataLogConfigurationFile.fileName() << ":" << error.what();
        return false;
    }

    // Load driver search paths.
    auto driverSearchPaths = settings.driverSearchPaths().split(" ");
    qCInfo(DAEMON,) << "Driver search locations are" << driverSearchPaths;

    // Load Storage driver.
    auto storageDriverName = settings.storageDriver();
    auto* storageDriver = SIStorageDriver::loadStorageDriver(driverSearchPaths, storageDriverName);
    if (storageDriver == nullptr) {
        qCCritical(DAEMON,) << "Unable to load storage driver" << storageDriverName;
        return false;
    }
    storage_.reset(storageDriver->createStorageInstance(settings.storageOptions()));
    if (storage_ == nullptr) {
        qCCritical(DAEMON,) << "Unable to instantiate storage driver" << storageDriverName;
        return false;
    }
    qCInfo(DAEMON,) << "Successfully loaded storage driver" << storageDriverName;

    // Load or instantiate Authorize driver.
    if (settings.authorizeEnabled()) {
        qCInfo(DAEMON,) << "User authorization is enabled";

        auto authorizerDriverName = settings.authorizeDriver();
        if (authorizerDriverName == "Internal") {
            auto textFileAuthorizer = new SITextFileUserManagement();
            textFileAuthorizer->setFilename(configurationFileLocation + "/users.txt");
            authorizer_.reset(textFileAuthorizer);
            qCInfo(DAEMON,) << "Using internal text file authorize driver";
        } else {
            auto* authorizeDriver = SIUserAuthorizeDriver::loadUserAuthorizeDriver(driverSearchPaths, authorizerDriverName);
            if (authorizeDriver == nullptr) {
                qCCritical(DAEMON,) << "Unable to load authorize driver" << authorizerDriverName;
                return false;
            }
            authorizer_.reset(authorizeDriver->createUserAuthorizerInstance(settings.authorizeOptions()));
            if (authorizer_ == nullptr) {
                qCCritical(DAEMON,) << "Unable to instantiate authorize driver" << authorizerDriverName;
                return false;
            }
            qCInfo(DAEMON,) << "Successfully loaded authorize driver" << authorizeDriver;
        }
    } else {
        qCInfo(DAEMON,) << "User authorization is disabled, all users have" << to_string(settings.authorizeGuestAccessLevel()) << "access level";
    }

    // Load and instantiate configured device access drivers.
    qCInfo(DAEMON,) << "Loading device access drivers and instantiating objects...";
    for (const auto& deviceAccessConfiguration: settings.deviceAccessConfigurationNames()) {
        auto deviceAccessDriverName = settings.deviceAccessConfigurationDriver(deviceAccessConfiguration);
        if (SIDeviceAccessRegistry::loadDeviceAccessDriver(driverSearchPaths, deviceAccessDriverName)) {
            if (SIDeviceAccessRegistry::sharedRegistry().instantiateDeviceAccess(deviceAccessDriverName, deviceAccessConfiguration, settings.deviceAccessDriverOptions(deviceAccessConfiguration))) {
                qCInfo(DAEMON,) << "  - Successfully loaded and instantiated" << deviceAccessConfiguration << "with driver" << deviceAccessDriverName;
            } else {
                qCritical() << "  - Error creating device access" << deviceAccessConfiguration;
            }
        } else {
            qCritical() << "  - Error loading device access driver" << deviceAccessDriverName;
        }
    }

    // Create device access manager and start polling timer.
    int propertyPollInterval = settings.propertyPollInterval();
    deviceAccessManager_ = new SISequentialPropertyManager(this);
    deviceAccessManager_->startPropertyPolling(propertyPollInterval);

    // Create data log manager.
    dataLogManager_ = new SIDataLogManager(dataLogConfiguration, this, this);
    dataLogManager_->startPropertyPolling();

    // Enumerate all devices on startup.
    qCInfo(DAEMON,) << "Enumerating all devices...";
    QElapsedTimer elapsedTimer;
    elapsedTimer.start();
    auto deviceCount = SIDeviceAccessRegistry::sharedRegistry().enumerateDevices();
    if (deviceCount >= 0) {
        qCInfo(DAEMON,) << "  - Enumerated" << deviceCount << "devices in" << elapsedTimer.elapsed() << "ms.";
    } else {
        qCCritical(DAEMON,) << "Failed to enumerate devices";
    }

    // Create web socket manager.
    if (settings.webSocketEnabled()) {
        webSocketManager_ = new SIWebSocketManager(this, this);
        if (!webSocketManager_->listen(settings.webSocketPort())) {
            qCCritical(DAEMON,) << "Failed to start web socket listening";
            delete webSocketManager_;
        }
    }

    // Create bluetooth manager.
    if (settings.bluetoothEnabled()) {
        bluetoothManager_ = new SIBluetoothManager(this, this);
        bluetoothManager_->setPeripheralName(settings.bluetoothName());
        bluetoothManager_->startAdvertise();
    }

    return true;
}

const SISettings& SIDaemon::settings() const {
    return SISettings::sharedSettings();
}

SIDeviceAccessManager& SIDaemon::deviceAccessManager() {
    return *deviceAccessManager_;
}

const SIUserAuthorizer* SIDaemon::userAuthorizer() {
    return authorizer_.get();
}

SIStorage& SIDaemon::storage() {
    return *storage_;
}
