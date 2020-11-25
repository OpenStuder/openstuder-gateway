#pragma once
#include <sideviceaccessdriver.h>
#include <QObject>

class XCom485iDriver: public QObject, public SIDeviceAccessDriver {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIDeviceAccessDriverPlugin_IID FILE "xcom485idriver.json")
    Q_INTERFACES(SIDeviceAccessDriver)

  public:
    SIDeviceAccess* createDeviceAccessInstance(const QString& id, const QVariantMap& parameters) override;
};
