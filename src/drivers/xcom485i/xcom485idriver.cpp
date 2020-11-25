#include "xcom485idriver.h"
#include "xcom485ideviceaccess.h"

SIDeviceAccess* XCom485iDriver::createDeviceAccessInstance(const QString& id, const QVariantMap& parameters) {
    if (!parameters.contains("port") || !parameters["port"].canConvert<QString>()) {
        return nullptr;
    }
    auto port = parameters["port"].toString();

    if (!parameters.contains("baudRate") || !parameters["baudRate"].canConvert<unsigned int>()) {
        return nullptr;
    }
    auto baudRate = parameters.value("baudRate", 9600).toUInt();

    auto deviceOffset = parameters.value("unitOffset", 0).toUInt();

    auto* instance = new XCom485iDeviceAccess(id);
    if (!instance->open(port, baudRate, deviceOffset)) {
        delete instance;
        return nullptr;
    }

    return instance;
}
