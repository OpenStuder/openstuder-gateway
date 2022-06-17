#pragma once
#include "siabstractbluetoothprotocol.h"
#include "../extension//sisessioncontext.h"
#include <siaccesslevel.h>

class SIBluetoothProtocolV1: public SIAbstractBluetoothProtocol, SISessionContext {
    Q_OBJECT

  public:
    explicit SIBluetoothProtocolV1(QString username, SIAccessLevel accessLevel);

    SIBluetoothProtocolFrame handleFrame(SIBluetoothProtocolFrame& frame, SIContext& context) override;
    SIBluetoothProtocolFrame convertDeviceMessage(const SIDeviceMessage& message) override;

    void propertyChanged(SIGlobalPropertyID id, const QVariant& value) override;

  private slots:
    void enumerationOperationFinished_(SIStatus status);
    void readPropertyOperationFinished_(SIStatus status);
    void writePropertyOperationFinished_(SIStatus status);
    void extensionCallCompleted_();

  private:
    QString username() const override;
    SIAccessLevel accessLevel() override;

    QString username_;
    SIAccessLevel accessLevel_;
};
