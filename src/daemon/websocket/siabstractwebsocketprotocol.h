#pragma once
#include <QObject>
#include "siwebsocketprotocolframe.h"
#include "../deviceaccess/sideviceaccessmanager.h"

class SIAbstractWebSocketProtocol: public QObject, public SIDeviceAccessManager::PropertySubscriber {
  Q_OBJECT

  public:
    virtual SIWebSocketProtocolFrame handleFrame(SIWebSocketProtocolFrame& frame, SIDeviceAccessManager* deviceAccessManager) = 0;
    virtual SIWebSocketProtocolFrame convertDeviceMessage(const QString& deviceAccessID, const SIDeviceMessage& message) = 0;

  signals:
    void frameReadyToSend(const SIWebSocketProtocolFrame& frame);
};
