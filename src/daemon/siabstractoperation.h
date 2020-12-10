#pragma once
#include <QObject>
#include <sistatus.h>

class SIDeviceAccessRegistry;

class SIAbstractOperation: public QObject {
    Q_OBJECT

  public:
    inline explicit SIAbstractOperation(QObject* parent = nullptr): QObject(parent) {}
    ~SIAbstractOperation() override = default;

  signals:
    void finished(SIStatus status);

  public slots:
    void execute();
    void execute(SIDeviceAccessRegistry* deviceAccessRegistry);

  private:
    virtual void execute_(SIDeviceAccessRegistry* deviceAccessRegistry) = 0;
};
