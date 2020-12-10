#pragma once
#include "sijsonflags.h"
#include <memory>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QPointer>
#include <QVariantMap>

class SIDeviceAccess;
class SIDeviceAccessDriver;

class SIDeviceAccessRegistry final: QObject {
  public:
    SIDeviceAccessRegistry();
    SIDeviceAccessRegistry(const SIDeviceAccessRegistry&) = delete;
    SIDeviceAccessRegistry& operator =(const SIDeviceAccessRegistry&) = delete;
    ~SIDeviceAccessRegistry() override;

    int enumerateDevices();

    int deviceAccessCount() const;

    QPointer<SIDeviceAccess> deviceAccess(int index) const;

    inline QPointer<SIDeviceAccess> operator [](int index) const {
        return deviceAccess(index);
    }

    QPointer<SIDeviceAccess> deviceAccess(const QString& id) const;

    inline QPointer<SIDeviceAccess> operator [](const QString& id) const {
        return deviceAccess(id);
    }

    bool instantiateDeviceAccess(const QString& driverName, const QString& id, const QVariantMap& parameters);

    QJsonObject jsonDescription(SIJsonFlags flags = SIJsonFlag::Default) const;

    static SIDeviceAccessRegistry& sharedRegistry();

    static bool registerDeviceAccessDriver(const QString& name, const QJsonObject& metaData, SIDeviceAccessDriver* deviceAccessDriver);
    static bool loadDeviceAccessDriver(QString driverFile);
    static bool loadDeviceAccessDriver(const QStringList& driverSearchPaths, QString driverName);
    static int loadDeviceAccessDriversInFolder(const QString& driversFolderPath);

  private:
    static bool hasDeviceAccessDriver_(const QString& name);

    struct Private_;
    std::unique_ptr<Private_> priv_;
};
