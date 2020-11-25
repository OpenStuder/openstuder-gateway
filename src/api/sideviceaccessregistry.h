#pragma once
#include <sijsonflags.h>
#include <memory>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QPointer>

class SIDeviceAccess;
class SIDeviceAccessDriver;

class SIDeviceAccessRegistry final: QObject {
    friend class SIDeviceAccess;

  public:
    SIDeviceAccessRegistry();
    SIDeviceAccessRegistry(const SIDeviceAccessRegistry&) = delete;
    SIDeviceAccessRegistry& operator =(const SIDeviceAccessRegistry&) = delete;
    ~SIDeviceAccessRegistry() override;

    int deviceAccessCount() const;

    QPointer<SIDeviceAccess> deviceAccess(int index) const;

    inline QPointer<SIDeviceAccess> operator [](int index) const {
        return deviceAccess(index);
    }

    QPointer<SIDeviceAccess> deviceAccess(const QString& id) const;

    inline QPointer<SIDeviceAccess> operator [](const QString& id) const {
        return deviceAccess(id);
    }

    QJsonObject jsonDescription(SIJsonFlags flags = SIJsonFlag::Default) const;

    static SIDeviceAccessRegistry& sharedRegistry();

    static bool registerDeviceAccessDriver(const QString& name, const QJsonObject& metaData, SIDeviceAccessDriver* deviceAccessFactory);
    static bool loadDeviceAccessDriver(const QString& driverFile);
    static int loadDeviceAccessDriversInFolder(const QString& driversFolderPath);

  private:
    void registerDeviceAccessInstance_(SIDeviceAccess* access);

    struct Private_;
    std::unique_ptr<Private_> priv_;
};
