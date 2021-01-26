#include "sidaemon.h"
#include <sistoragedriver.h>
#include <sidevice.h>
#include <sideviceaccess.h>
#include <sideviceaccessregistry.h>
#include <siuserauthorizedriver.h>
#include <sitextfileusermanagement.h>
#include "deviceaccess/sideviceaccessmanager.h"
#include "deviceaccess/sisequentialpropertymanager.h"
#include "websocket/siwebsocketmanager.h"
#include "bluetooth/sibluetoothmanager.h"
#include "sisettings.h"
#include <QCommandLineParser>
#include <QFile>
#include <QLoggingCategory>
#include <QSettings>
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
    parser.addVersionOption();
    QCommandLineOption configurationFileOption {{"c", "config"}, "Use configuration location <dir>", "dir"};
    configurationFileOption.setDefaultValue(OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION);
    parser.addOption(configurationFileOption);
    parser.process(*this);

    // Load configuration file.
    auto configurationFileLocation = parser.value(configurationFileOption);
    try {
        SISettings::loadFromLocation(configurationFileLocation);
    } catch (std::runtime_error& error) {
        qCCritical(DAEMON,) << error.what();
        return false;
    }
    qCInfo(DAEMON,) << "Using configuration directory" << configurationFileLocation;

    // Load configuration file.
    auto& settings = SISettings::sharedSettings();

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
            authorizer_ = std::make_unique<SITextFileUserManagement>();
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

    // Create property manager and start polling timer.
    int propertyPollInterval = settings.propertyPollInterval();
    deviceAccessManager_ = new SISequentialPropertyManager(this);
    deviceAccessManager_->startPropertyPolling(propertyPollInterval);

    // Create web socket manager.
    if (settings.webSocketEnabled()) {
        webSocketManager_ = new SIWebSocketManager(deviceAccessManager_, this);
        if (!webSocketManager_->listen(settings.webSocketPort())) {
            qCCritical(DAEMON,) << "Failed to start web socket listening";
            delete webSocketManager_;
        }
    }

    // Create bluetooth manager.
    if (settings.bluetoothEnabled()) {
        bluetoothManager_ = new SIBluetoothManager(deviceAccessManager_, this);
        bluetoothManager_->setPeripheralName(settings.bluetoothName());
        bluetoothManager_->startAdvertise();
    }

    return true;
}
