#include "usermanagementextensiondriver.h"
#include "usermanagementextension.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(UserManagement, "driver.extension.UserManagement", QtInfoMsg)

SIExtension* UserManagementExtensionDriver::createExtensionInstance(const QVariantMap& parameters) {
    auto allowedUsers = parameters.value("allowedUsers").toStringList();
    if (allowedUsers.isEmpty()) {
        qCCritical(UserManagement,) << "Missing required parameter \"allowedUsers\"";
        return nullptr;
    }

    for (auto& user: allowedUsers) {
        user = user.trimmed();
    }

    return new UserManagementExtension(allowedUsers);
}
