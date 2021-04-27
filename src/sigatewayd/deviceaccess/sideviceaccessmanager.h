#pragma once
#include "operations/sideviceenumerationoperation.h"
#include "operations/sipropertyreadoperation.h"
#include "operations/sipropertywriteoperation.h"
#include "operations/sipropertiesreadoperation.h"
#include <sipropertyid.h>
#include <sipropertyreadresult.h>
#include <sipropertywriteresult.h>
#include <sipropertywriteflags.h>
#include <sidevicemessage.h>
#include <siproperty.h>
#include <sidevice.h>
#include <sideviceaccess.h>
#include <QObject>
#include <QVector>

class SIDeviceAccessManager: public QObject {
    Q_OBJECT

  public:
    class PropertySubscriber {
      public:
        virtual ~PropertySubscriber() = default;
        virtual void propertyChanged(SIGlobalPropertyID id, const QVariant& value) = 0;
    };

    explicit SIDeviceAccessManager(QObject* parent = nullptr);
    ~SIDeviceAccessManager() override;

    SIProperty resolveProperty(const SIGlobalPropertyID& id);
    QVector<SIGlobalPropertyID> findProperties(SIGlobalPropertyID propertyID);

    SIDeviceEnumerationOperation* enumerateDevices();
    SIPropertyReadOperation* readProperty(const SIGlobalPropertyID& id);
    SIPropertiesReadOperation* readProperties(const QVector<SIGlobalPropertyID>& ids);
    SIPropertyWriteOperation* writeProperty(const SIGlobalPropertyID& id, const QVariant& value = {}, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default);

    bool subscribeToProperty(const SIGlobalPropertyID& id, PropertySubscriber* subscriber);
    bool unsubscribeFromProperty(const SIGlobalPropertyID& id, PropertySubscriber* subscriber);
    void unsubscribeFromAllProperties(PropertySubscriber* subscriber);

    void startPropertyPolling(int intervalMS);

  signals:
    void deviceMessageReceived(const SIDeviceMessage& message);
    void deviceAdded(const SIDeviceAccess& access, const SIDevice& device);
    void deviceRemoved(const SIDeviceAccess& access, const SIDevice& device);

  private slots:
    void onDeviceAdded_(const SIDevice& device);
    void onDeviceRemoved_(const SIDevice& device);

  private:
    void timerEvent(QTimerEvent* event) override;
    virtual void enqueueOperation_(SIAbstractOperation* operation) = 0;

    struct Private;
    Private* priv_;
};
