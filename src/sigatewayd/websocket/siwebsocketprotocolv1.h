#pragma once
#include "siabstractwebsocketprotocol.h"
#include <siaccesslevel.h>

class SIWebSocketProtocolV1: public SIAbstractWebSocketProtocol {
    Q_OBJECT

  public:
    SIWebSocketProtocolV1(SIAccessLevel accessLevel);

    SIWebSocketProtocolFrame handleFrame(SIWebSocketProtocolFrame& frame, SIContext& context) override;
    SIWebSocketProtocolFrame convertDeviceMessage(const SIDeviceMessage& message) override;

    void propertyChanged(SIGlobalPropertyID id, const QVariant& value) override;

  private slots:
    void enumerationOperationFinished_(SIStatus status);
    void readPropertyOperationFinished_(SIStatus status);
    void writePropertyOperationFinished_(SIStatus status);

  private:
    SIAccessLevel accessLevel_;
};
