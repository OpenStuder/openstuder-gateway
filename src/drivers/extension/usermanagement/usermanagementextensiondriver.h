#pragma once
#include <siextensiondriver.h>

class UserManagementExtensionDriver: public QObject, public SIExtensionDriver {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIExtensionDriver_IID FILE "usermanagementextensiondriver.json")
    Q_INTERFACES(SIExtensionDriver)

  public:
    SIExtension* createExtensionInstance(const QVariantMap& parameters) override;
};
