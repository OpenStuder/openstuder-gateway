#pragma once
#include "sicontext.h"
#include <QCoreApplication>
#include <memory>

class SIStorage;
class SIUserAuthorizer;
class QSettings;
class SIDeviceAccessManager;
class SIWebSocketManager;
class SIBluetoothManager;
class SIDataLogManager;

class SIDaemon: public QCoreApplication, public SIContext {
    Q_OBJECT

  public:
    SIDaemon(int& argc, char** args);
    ~SIDaemon() override;

    bool initialize();

  private:
    const SISettings& settings() const override;
    SIDeviceAccessManager& deviceAccessManager() override;
    const SIUserAuthorizer* userAuthorizer() override;
    SIStorage& storage() override;

    SIDeviceAccessManager* deviceAccessManager_ = nullptr;
    std::unique_ptr<SIUserAuthorizer> authorizer_;
    std::unique_ptr<SIStorage> storage_;

    SIDataLogManager* dataLogManager_ = nullptr;
    SIWebSocketManager* webSocketManager_ = nullptr;
    SIBluetoothManager* bluetoothManager_ = nullptr;
};
