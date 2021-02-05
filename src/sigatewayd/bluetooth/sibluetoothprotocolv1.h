#pragma once
#include "siabstractbluetoothprotocol.h"
#include <siaccesslevel.h>

class SIBluetoothProtocolV1: public SIAbstractBluetoothProtocol {
    Q_OBJECT

  public:
    SIBluetoothProtocolV1(SIAccessLevel accessLevel);

    SIBluetoothProtocolFrame handleFrame(SIBluetoothProtocolFrame& frame, SIContext& context) override;
    SIBluetoothProtocolFrame convertDeviceMessage(const SIDeviceMessage& message) override;

    void propertyChanged(SIGlobalPropertyID id, const QVariant& value) override;

  private slots:
    void enumerationOperationFinished_(SIStatus status);
    void readPropertyOperationFinished_(SIStatus status);
    void writePropertyOperationFinished_(SIStatus status);

  private:
    SIAccessLevel accessLevel_;
};
