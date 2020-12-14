#pragma once
#include <QObject>
#include "../deviceaccess/sideviceaccessmanager.h"

class QLowEnergyController;
class SIDeviceAccessManager;
class QLowEnergyCharacteristic;
class QLowEnergyService;

  class SIBluetoothManager: public QObject, public SIDeviceAccessManager::PropertySubscriber {
  Q_OBJECT

  public:
    explicit SIBluetoothManager(SIDeviceAccessManager* deviceAccessManager, QObject* parent = nullptr);

      const QString& name() const {
          return name_;
      }

      void setName(const QString& name) {
          name_ = name;
      }

    public slots:
    void startAdvertise();

  private slots:
    void onCharacteristicChanged_(const QLowEnergyCharacteristic& characteristic, const QByteArray& value);
    void onDisconnected_();

  private:
    void propertyChanged(SIGlobalPropertyID id, const QVariant& value) override;

    QString name_;
    QLowEnergyController* peripheral_;
    QLowEnergyService* service_;
    SIDeviceAccessManager* deviceAccessManager_;
};
