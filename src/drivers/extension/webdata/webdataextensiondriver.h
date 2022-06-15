#pragma once
#include <siextensiondriver.h>

class WebDataExtensionDriver: public QObject, public SIExtensionDriver {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIExtensionDriver_IID FILE "webdataextensiondriver.json")
    Q_INTERFACES(SIExtensionDriver)

  public:
    SIExtension* createExtensionInstance(const QVariantMap& parameters) override;
};
