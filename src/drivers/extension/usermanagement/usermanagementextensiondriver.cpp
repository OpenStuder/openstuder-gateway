#include "usermanagementextensiondriver.h"
#include "usermanagementextension.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(UserManagement, "driver.extension.UserManagement", QtInfoMsg)

SIExtension* UserManagementExtensionDriver::createExtensionInstance(const QVariantMap& parameters) {
    qCCritical(UserManagement,) << parameters;
    auto adminUsers = parameters.value("adminUsers").toStringList();
    if (adminUsers.isEmpty()) {
        qCCritical(UserManagement,) << "Missing required parameter \"adminUsers\"";
        return nullptr;
    }
    qCCritical(UserManagement,) << adminUsers;
    for (auto& adminUser: adminUsers) {
        adminUser = adminUser.trimmed();
    }

    return new UserManagementExtension(adminUsers);
}
