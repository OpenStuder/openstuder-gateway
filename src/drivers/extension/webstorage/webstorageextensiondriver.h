#pragma once
#include <siextensiondriver.h>

class WebStorageExtensionDriver: public QObject, public SIExtensionDriver {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIExtensionDriver_IID FILE "webstorageextensiondriver.json")
    Q_INTERFACES(SIExtensionDriver)

  public:
    SIExtension* createExtensionInstance(const QVariantMap& parameters) override;
};
