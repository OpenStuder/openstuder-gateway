#pragma once
#include <siextension.h>
#include <siaccesslevel.h>

class UserManagementExtension: public SIExtension {
  Q_OBJECT

  public:
    explicit UserManagementExtension(QStringList allowedUsers);
    ~UserManagementExtension() override;

  private:
    QStringList& commands_() const override;
    SIExtensionWebSocketResult* runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) override;

    bool bluetoothSupported_() const override;
    SIExtensionBluetoothResult* runCommand_(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters) override;
};
