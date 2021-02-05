#pragma once
#include <QObject>
#include "../sicontext.h"
#include "siwebsocketprotocolframe.h"
#include "../deviceaccess/sideviceaccessmanager.h"

class SIAbstractWebSocketProtocol: public QObject, public SIDeviceAccessManager::PropertySubscriber {
    Q_OBJECT

  public:
    virtual SIWebSocketProtocolFrame handleFrame(SIWebSocketProtocolFrame& frame, SIContext& context) = 0;
    virtual SIWebSocketProtocolFrame convertDeviceMessage(const SIDeviceMessage& message) = 0;

  signals:
    void frameReadyToSend(const SIWebSocketProtocolFrame& frame);
};
