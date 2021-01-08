#pragma once
#include <QObject>
#include "sibluetoothprotocolframe.h"
#include "../deviceaccess/sideviceaccessmanager.h"

class SIAbstractBluetoothProtocol: public QObject, public SIDeviceAccessManager::PropertySubscriber {
    Q_OBJECT

  public:
    virtual SIBluetoothProtocolFrame handleFrame(SIBluetoothProtocolFrame& frame, SIDeviceAccessManager* deviceAccessManager) = 0;
    virtual SIBluetoothProtocolFrame convertDeviceMessage(const QString& deviceAccessID, const SIDeviceMessage& message) = 0;

  signals:
    void frameReadyToSend(const SIBluetoothProtocolFrame& frame);
};
