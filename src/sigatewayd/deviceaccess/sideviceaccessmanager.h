#pragma once
#include "operations/sideviceenumerationoperation.h"
#include "operations/sipropertyreadoperation.h"
#include "operations/sipropertywriteoperation.h"
#include <sipropertyid.h>
#include <sipropertyreadresult.h>
#include <sipropertywriteresult.h>
#include <sipropertywriteflags.h>
#include <sidevicemessage.h>
#include <siproperty.h>
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

    SIProperty resolveProperty(SIGlobalPropertyID id);

    SIDeviceEnumerationOperation* enumerateDevices();
    SIPropertyReadOperation* readProperty(SIGlobalPropertyID id);
    SIPropertyWriteOperation* writeProperty(SIGlobalPropertyID id, const QVariant& value = {}, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default);

    bool subscribeToProperty(SIGlobalPropertyID id, PropertySubscriber* subscriber);
    bool unsubscribeFromProperty(SIGlobalPropertyID id, PropertySubscriber* subscriber);
    void unsubscribeFromAllProperties(PropertySubscriber* subscriber);

    void startPropertyPolling(int intervalMS);

  signals:
    void deviceMessageReceived(const QString& deviceAccessID, const SIDeviceMessage& message);

  private:
    void timerEvent(QTimerEvent* event) override;
    virtual void enqueueOperation_(SIAbstractOperation* operation) = 0;

    struct Private;
    Private* priv_;
};
