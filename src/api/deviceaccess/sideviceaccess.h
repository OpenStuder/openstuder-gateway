#pragma once
#include "siaccesslevel.h"
#include "sijdescriptionflags.h"
#include "sidevicemessage.h"
#include <memory>
#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QVector>
#include <QPointer>

class SIDevice;
class SIDeviceAccessRegistry;

class SIDeviceAccess: public QObject {
    Q_OBJECT

  public:
    SIDeviceAccess(const SIDeviceAccess&) = delete;
    SIDeviceAccess& operator =(const SIDeviceAccess&) = delete;
    ~SIDeviceAccess() override;

    const QString& id() const;

    QVector<SIDeviceMessage> retrievePendingDeviceMessages() const;

    int enumerateDevices();

    int deviceCount() const;

    QPointer<SIDevice> device(int index) const;

    inline QPointer<SIDevice> operator [](int index) const {
        return device(index);
    }

    QPointer<SIDevice> device(const QString& id) const;

    inline QPointer<SIDevice> operator [](const QString& id) {
        return device(id);
    }

    QJsonObject jsonDescription(SIAccessLevel accessLevel, SIDescriptionFlags flags) const;

  signals:
    void deviceAdded(const SIDevice& device);
    void deviceRemoved(const SIDevice& device);

  protected:
    explicit SIDeviceAccess(const QString& id);

  private:
    virtual void retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const;

    virtual bool enumerateDevices_(QVector<SIDevice*>& devices) = 0;

    virtual void completeJsonDescription_(QJsonObject& object, SIDescriptionFlags flags) const;

    struct Private_;
    std::unique_ptr<Private_> private_;
};
