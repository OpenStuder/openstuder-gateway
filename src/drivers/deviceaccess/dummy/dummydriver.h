#pragma once
#include <QObject>
#include <sideviceaccessdriver.h>

class DummyDriver: public QObject, public SIDeviceAccessDriver {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIDeviceAccessDriverPlugin_IID FILE "dummydriver.json")
    Q_INTERFACES(SIDeviceAccessDriver)

  public:
    SIDeviceAccess* createDeviceAccessInstance(const QString& id, const QVariantMap& parameters) override;
};
