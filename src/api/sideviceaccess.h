#pragma once
#include <sijsonflags.h>
#include <memory>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QVector>

class SIDevice;
class SIDeviceAccessRegistry;

class SIDeviceAccess {
  public:
    SIDeviceAccess(const SIDeviceAccess&) = delete;
    SIDeviceAccess& operator =(const SIDeviceAccess&) = delete;
    virtual ~SIDeviceAccess();

    const QString& id() const;

    int enumerateDevices();

    int deviceCount() const;

    std::weak_ptr<SIDevice> device(int index) const;

    inline std::weak_ptr<SIDevice> operator [](int index) const {
        return device(index);
    }

    std::weak_ptr<SIDevice> device(const QString& id) const;

    inline std::weak_ptr<SIDevice> operator [](const QString& id) {
        return device(id);
    }

    const QJsonObject& jsonDescription(SIJsonFlags flags) const;

  protected:
    explicit SIDeviceAccess(const QString& id, SIDeviceAccessRegistry* registry = nullptr);

  private:
    virtual bool enumerateDevices_(QVector<std::shared_ptr<SIDevice>>& devices) = 0;

    virtual void completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const;

    struct Private_;
    std::unique_ptr<Private_> priv_;
};
