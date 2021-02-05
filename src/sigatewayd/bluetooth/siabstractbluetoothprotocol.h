#pragma once
#include <QObject>
#include "../sicontext.h"
#include "sibluetoothprotocolframe.h"
#include "../deviceaccess/sideviceaccessmanager.h"

class SIAbstractBluetoothProtocol: public QObject, public SIDeviceAccessManager::PropertySubscriber {
    Q_OBJECT

  public:
    virtual SIBluetoothProtocolFrame handleFrame(SIBluetoothProtocolFrame& frame, SIContext& context) = 0;
    virtual SIBluetoothProtocolFrame convertDeviceMessage(const SIDeviceMessage& message) = 0;

  signals:
    void frameReadyToSend(const SIBluetoothProtocolFrame& frame);
};
