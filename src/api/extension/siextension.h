#pragma once
#include "siextensioncontext.h"
#include "siextensionwebsocketresult.h"
#include "siextensionbluetoothresult.h"
#include <memory>
#include <QObject>
#include <QString>
#include <QPair>
#include <QByteArray>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QSet>

class SIExtension: public QObject {
  public:
    SIExtension(const SIExtension&) = delete;
    SIExtension& operator =(const SIExtension&) = delete;
    ~SIExtension() override;

    bool allowedForUser(const QString& sessionUsername) const;

    void setAllowedUsers(const QStringList& allowedUsers);

    const QString& id() const;

    const QStringList& commands() const;

    const SIExtensionWebSocketResult* runCommand(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body);

    bool bluetoothSupported() const;
    const SIExtensionBluetoothResult* runCommand(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters);

  protected:
    explicit SIExtension(const QString& id, const QStringList allowedUsers = {});

    static bool validateWebSocketHeaders(const QMap<QString, QString>& headers, const std::initializer_list<const char*>& required, const std::initializer_list<const char*>& optional = {});
    static bool validateBluetoothParameters(const QVector<QVariant>& parameters, const QVector<QSet<QVariant::Type>>& parameterTypes, bool allowAdditionalParameters = false);

  private:
    virtual QStringList& commands_() const = 0;

    virtual SIExtensionWebSocketResult* runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) = 0;

    virtual bool bluetoothSupported_() const { return false; }
    virtual SIExtensionBluetoothResult* runCommand_(const SIExtensionContext& context, const QString& command, const QVector<QVariant>& parameters) {
        Q_UNUSED(context);
        Q_UNUSED(command);
        Q_UNUSED(parameters);
        return new SIExtensionBluetoothResult(SIExtensionStatus::UnsupportedExtension);
    };

    struct Private_;
    std::unique_ptr<Private_> private_;
};
