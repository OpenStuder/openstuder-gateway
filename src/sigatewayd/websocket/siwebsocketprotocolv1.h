#pragma once
#include "siabstractwebsocketprotocol.h"
#include <siaccesslevel.h>
#include "../extension/sisessioncontext.h"


class SIWebSocketProtocolV1: public SIAbstractWebSocketProtocol, SISessionContext {
    Q_OBJECT

  public:
    explicit SIWebSocketProtocolV1(QString username, SIAccessLevel accessLevel);

    SIWebSocketProtocolFrame handleFrame(SIWebSocketProtocolFrame& frame, SIContext& context) override;
    SIWebSocketProtocolFrame convertDeviceMessage(const SIDeviceMessage& message) override;

    void propertyChanged(SIGlobalPropertyID id, const QVariant& value) override;

  private slots:
    void enumerationOperationFinished_(SIStatus status);
    void readPropertyOperationFinished_(SIStatus status);
    void readPropertiesOperationFinished_(SIStatus status);
    void writePropertyOperationFinished_(SIStatus status);

  private:
    QString username() const override;
    SIAccessLevel accessLevel() override;

    QString username_;
    SIAccessLevel accessLevel_;
};
