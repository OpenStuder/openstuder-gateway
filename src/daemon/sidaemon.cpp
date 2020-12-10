#include "sidaemon.h"
#include <sistoragedriver.h>
#include <sidevice.h>
#include <sideviceaccess.h>
#include <sideviceaccessregistry.h>
#include "siabstractpropertymanager.h"
#include "sisequentialpropertymanager.h"
#include <QtCore/QCommandLineParser>
#include <QtCore/QFile>
#include <QLoggingCategory>
#include <QtCore/QSettings>
#include <QtCore/QElapsedTimer>
#include <unistd.h>

Q_LOGGING_CATEGORY(DAEMON, "", QtWarningMsg)

SIDaemon::SIDaemon(int argc, char** argv): QCoreApplication(argc, argv) {
    setApplicationName("sigatewayd");
}

SIDaemon::~SIDaemon() = default;

bool SIDaemon::initialize() {
    // Parse command line options.
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption configurationFileOption {{"c", "config"}, "Use configuration file <file> instead of default \"/etc/sigateway.conf\"", "file"};
    configurationFileOption.setDefaultValue("/etc/sigatewayd.conf");
    parser.addOption(configurationFileOption);
    parser.process(*this);

    // Ensure that configuration file exists.
    auto configurationFileLocation = parser.value(configurationFileOption);
    if (!QFile::exists(configurationFileLocation)) {
        qCCritical(DAEMON) << "Configuration file" << configurationFileLocation << "does not exist";
        return false;
    }

    // Load configuration file.
    QSettings settings(parser.value(configurationFileOption), QSettings::IniFormat);
    if (settings.status() != QSettings::NoError) {
        qCritical(DAEMON) << "Configuration file" << configurationFileLocation << "is malformed";
        return false;
    }

    // Load driver search paths.
    settings.beginGroup("Gateway");
    auto driverSearchPaths = settings.value("driverSearchPaths", "/var/lib/sigatewayd/drivers").toString().split(" ");
    settings.endGroup();

    // Load Storage driver.
    settings.beginGroup("Storage");
    auto storageDriverName = settings.value("driver", "SQLite").toString();
    auto* storageDriver = SIStorageDriver::loadStorageDriver(driverSearchPaths, storageDriverName);
    if (storageDriver == nullptr) {
        qCritical() << "Unable to load storage driver" << storageDriverName;
        return false;
    }
    storage_.reset(storageDriver->createStorageInstance(filteredChildSettings_(settings, {"driver"})));
    if (storage_ == nullptr) {
        qCritical() << "Unable to instantiate storage driver" << storageDriverName;
        return false;
    }
    settings.endGroup();

    // Load and instantiate configured device access drivers.
    for (const auto& group: filteredChildGroups_(settings, {"Gateway", "Storage"})) {
        settings.beginGroup(group);
        auto deviceAccessDriverName = settings.value("driver").toString();
        if (!SIDeviceAccessRegistry::loadDeviceAccessDriver(driverSearchPaths, deviceAccessDriverName)) {
            qCritical() << "Error loading device access driver" << deviceAccessDriverName;
        } else {

        }if (!SIDeviceAccessRegistry::sharedRegistry().instantiateDeviceAccess(deviceAccessDriverName, group, filteredChildSettings_(settings, {"driver"}))) {
            qCritical() << "Error creating device access" << group;
        }
        settings.endGroup();
    }

    propertyManager_.reset(new SISequentialPropertyManager(this));

    /**********************
     * TEST, to be removed
     */
    storage_->storePropertyValue(22, 1.23);
    QTextStream out(stdout);

    auto messages = SIDeviceAccessRegistry::sharedRegistry().deviceAccess("xcom")->retrievePendingDeviceMessages();
    out << "MSG CNT = " << messages.count() << endl;
    for (const auto& message: messages) {
        out << "MSG id=" << message.messageID << ", device id=" << message.deviceID << ", message=" << message.message << endl;
    }

    QElapsedTimer t;
    t.start();
    SIDeviceAccessRegistry::sharedRegistry().enumerateDevices();
    qDebug() << "Enumerated in" << t.elapsed() << "ms.";

    auto readVoltage = propertyManager_->readProperty({"xcom", "10", 3000});
    QObject::connect(readVoltage, &SIAbstractOperation::finished, [=](SIStatus status) {
       qDebug() << "ASYNC READ status=" << (int)status << "Value =" << readVoltage->value().toFloat();
       delete readVoltage;
    });

    return true;







    auto virtualXtender = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(0)->device("10");

    // Read AC output power.
    auto result = virtualXtender->readProperty(3023);
    qDebug() << "Status =" << (int)result.status << "Power =" << (result.value.toFloat() * 1000.f) << "VA";

    // Turn on.
    virtualXtender->writeProperty(1415, {});
    sleep(5);

    // Read AC output power in ON state.
    result = virtualXtender->readProperty(3023);
    qDebug() << "Status =" << (int)result.status << "Power =" << (result.value.toFloat() * 1000.f) << "VA";

    // Turn off.
    virtualXtender->writeProperty(1399, {});
    sleep(5);

    // Read AC output power in OFF state.
    result = virtualXtender->readProperty(3023);
    qDebug() << "Status =" << (int)result.status << "Power =" << (result.value.toFloat() * 1000.f) << "VA";

    return true;
}

QVariantMap SIDaemon::filteredChildSettings_(const QSettings& settings, const QStringList& exclude) {
    QVariantMap filtered;
    for (const auto& key: settings.childKeys()) {
        if (!exclude.contains(key)) {
            filtered[key] = settings.value(key);
        }
    }
    return filtered;
}

QStringList SIDaemon::filteredChildGroups_(const QSettings& settings, const QStringList& exclude) {
    QStringList filtered;
    for (const auto& group: settings.childGroups()) {
        if (!exclude.contains(group)) {
            filtered << group;
        }
    }
    return filtered;
}
