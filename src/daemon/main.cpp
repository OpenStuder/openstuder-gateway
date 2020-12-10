#include <iostream>
#include <sidevice.h>
#include <sideviceaccess.h>
#include <sideviceaccessregistry.h>
#include <sistoragedriver.h>
#include <QtDebug>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QPluginLoader>
#include <unistd.h>
#include <QtCore/QCommandLineParser>
#include <QtCore/QSettings>
#include <QtCore/QFile>

QVariantMap filteredChildSettings(const QSettings& settings, const QStringList& exclude) {
    QVariantMap filtered;
    for (const auto& key: settings.childKeys()) {
        if (!exclude.contains(key)) {
            filtered[key] = settings.value(key);
        }
    }
    return filtered;
}

QStringList filteredChildGroups(const QSettings& settings, const QStringList& exclude) {
    QStringList filtered;
    for (const auto& group: settings.childGroups()) {
        if (!exclude.contains(group)) {
            filtered << group;
        }
    }
    return filtered;
}

int main(int argc, char** argv) {

    // Create and parametrize Qt application.
    QCoreApplication::setApplicationName("sigatewayd");
    QCoreApplication application {argc, argv};

    // Parse command line options.
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption configurationFileOption {{"c", "config"}, "Use configuration file <file> instead of default \"/etc/sigateway.conf\"", "file"};
    configurationFileOption.setDefaultValue("/etc/sigatewayd.conf");
    parser.addOption(configurationFileOption);
    parser.process(application);

    // Ensure that configuration file exists.
    auto configurationFileLocation = parser.value(configurationFileOption);
    if (!QFile::exists(configurationFileLocation)) {
        qCritical() << "Configuration file" << configurationFileLocation << "does not exist";
        return 1;
    }

    // Load configuration file.
    QSettings settings(parser.value(configurationFileOption), QSettings::IniFormat);
    if (settings.status() != QSettings::NoError) {
        qCritical() << "Configuration file" << configurationFileLocation << "is malformed";
        return 1;
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
        return 1;
    }
    auto* storage = storageDriver->createStorageInstance(filteredChildSettings(settings, {"driver"}));
    if (storage == nullptr) {
        qCritical() << "Unable to instantiate storage driver" << storageDriverName;
        return 1;
    }
    settings.endGroup();

    // Load and instantiate configured device access drivers.
    for (const auto& group: filteredChildGroups(settings, {"Gateway", "Storage"})) {
        settings.beginGroup(group);
        auto deviceAccessDriverName = settings.value("driver").toString();
        if (!SIDeviceAccessRegistry::loadDeviceAccessDriver(driverSearchPaths, deviceAccessDriverName)) {
            qCritical() << "Error loading device access driver" << deviceAccessDriverName;
        } else {

        }if (!SIDeviceAccessRegistry::sharedRegistry().instantiateDeviceAccess(deviceAccessDriverName, group, filteredChildSettings(settings, {"driver"}))) {
            qCritical() << "Error creating device access" << group;
        }
        settings.endGroup();
    }

    storage->storePropertyValue(22, 1.23);
    QTextStream out(stdout);

    /**********************
     * TEST, to be removed
     */

    auto messages = SIDeviceAccessRegistry::sharedRegistry().deviceAccess("xcom")->retrievePendingDeviceMessages();
    out << "MSG CNT = " << messages.count() << endl;
    for (const auto& message: messages) {
        out << "MSG id=" << message.messageID << ", device id=" << message.deviceID << ", message=" << message.message << endl;
    }

    QElapsedTimer t;
    t.start();
    SIDeviceAccessRegistry::sharedRegistry().enumerateDevices();
    qDebug() << "Enumerated in" << t.elapsed() << "ms.";

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
}
