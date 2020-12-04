#include "xcom485idriver.h"
#include "xcom485ideviceaccess.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(XCOM485i, "driver.XCom-485i", QtWarningMsg)

SIDeviceAccess* XCom485iDriver::createDeviceAccessInstance(const QString& id, const QVariantMap& parameters) {

    // Get and validate "port" parameter.
    if (!parameters.contains("port")) {
        qCCritical(XCOM485i, "Missing mandatory parameter \"port\"");
        return nullptr;
    }
    if (!parameters["port"].canConvert<QString>()) {
        qCCritical(XCOM485i) << "Invalid value for parameter \"port\", must by of type String";
        return nullptr;
    }
    auto port = parameters["port"].toString();

    // Get and validate "baudRate" parameter.
    bool ok;
    auto baudRate = parameters.value("baudRate", 9600).toUInt(&ok);
    if (!ok) {
        qCCritical(XCOM485i) << "Invalid value for parameter \"baudRate\", must by of type int";
        return nullptr;
    }

    // Get and validate "deviceAddressOffset" parameter.
    auto deviceAddressOffset = parameters.value("deviceAddressOffset", 0).toUInt(&ok);
    if (!ok || (deviceAddressOffset != 0 && deviceAddressOffset != 32 && deviceAddressOffset != 64 && deviceAddressOffset != 128)) {
        qCCritical(XCOM485i) << "Invalid value for parameter \"deviceAddressOffset\", must be one of the following values {0, 32, 64, 128}";
        return nullptr;
    }

    // Create a new instance.
    auto* instance = new XCom485iDeviceAccess(id);

    // Try to open the instance.
    if (!instance->open(port, baudRate, deviceAddressOffset)) {
        qCCritical(XCOM485i) << "Failed to create XCom-485i device access" << id << ": Unable to open port" << port;
        delete instance;
        return nullptr;
    }

    qCInfo(XCOM485i) << "Successfully created XCom-485i device access" << id << "at" << port << ", baudRate =" << baudRate << ", deviceAddressOffset ="
                     << deviceAddressOffset;
    return instance;
}
