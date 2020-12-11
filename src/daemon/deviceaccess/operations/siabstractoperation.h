#pragma once
#include <QObject>
#include <sistatus.h>

class SIDeviceAccessRegistry;

class SIAbstractOperation: public QObject {
    Q_OBJECT

  public:
    explicit SIAbstractOperation(QObject* parent = nullptr);
    ~SIAbstractOperation() override;

  signals:
    void finished(SIStatus status);

  public slots:
    void execute();
    void execute(SIDeviceAccessRegistry* deviceAccessRegistry);

  private:
    virtual SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) = 0;
};
