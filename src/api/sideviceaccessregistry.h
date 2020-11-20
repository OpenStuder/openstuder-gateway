#pragma once
#include <sijsonflags.h>
#include <memory>
#include <QString>
#include <QJsonObject>

class SIDeviceAccess;

class SIDeviceAccessRegistry final {
    friend class SIDeviceAccess;

  public:
    SIDeviceAccessRegistry();
    SIDeviceAccessRegistry(const SIDeviceAccessRegistry&) = delete;
    SIDeviceAccessRegistry& operator =(const SIDeviceAccessRegistry&) = delete;
    ~SIDeviceAccessRegistry();

    int deviceAccessCount() const;

    std::weak_ptr<SIDeviceAccess> deviceAccess(int index) const;

    inline std::weak_ptr<SIDeviceAccess> operator [](int index) const {
        return deviceAccess(index);
    }

    std::weak_ptr<SIDeviceAccess> deviceAccess(const QString& id) const;

    inline std::weak_ptr<SIDeviceAccess> operator [](const QString& id) const {
        return deviceAccess(id);
    }

    QJsonObject jsonDescription(SIJsonFlags flags = SIJsonFlag::Default) const;

    static SIDeviceAccessRegistry& sharedRegistry();

  private:
    void registerDeviceAccess_(SIDeviceAccess* access);

    struct Private_;
    std::unique_ptr<Private_> priv_;
};
