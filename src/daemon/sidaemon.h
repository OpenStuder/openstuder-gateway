#pragma once
#include <QCoreApplication>
#include <memory>

class SIStorage;
class QSettings;
class SIDeviceAccessManager;
class SIWebSocketManager;
class SIBluetoothManager;

class SIDaemon: public QCoreApplication {
    Q_OBJECT

  public:
    SIDaemon(int argc, char** args);
    ~SIDaemon() override;

    bool initialize();

  private:
    std::unique_ptr<SIStorage> storage_;
    SIDeviceAccessManager* deviceAccessManager_ = nullptr;
    SIWebSocketManager* webSocketManager_ = nullptr;
    SIBluetoothManager* bluetoothManager_ = nullptr;
};
