#pragma once
#include <siextension.h>
#include <siaccesslevel.h>
#include <QSqlDatabase>

class UserManagementExtension: public SIExtension {
    Q_OBJECT

  public:
    UserManagementExtension(QStringList adminUsers);
    ~UserManagementExtension() override;

  private:
    static Result list_(const SIExtensionContext& context);
    static Result add_(const SIExtensionContext& context, const QString& username, const QString& password, SIAccessLevel accessLevel);
    static Result changePassword_(const SIExtensionContext& context, const QString& username, const QString& password);
    static Result changeAccessLevel_(const SIExtensionContext& context, const QString& username, SIAccessLevel accessLevel);
    static Result remove_(const SIExtensionContext& context, const QString& username);

    QStringList& commands_() const override;
    Result runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) override;

    QStringList adminUsers_;
};
