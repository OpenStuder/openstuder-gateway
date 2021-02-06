#pragma once
#include <QObject>
#include <sistatus.h>
#include <functional>

class SIDeviceAccessRegistry;

class SIAbstractOperation: public QObject {
    Q_OBJECT

  public:
    using QueueFunction = std::function<void (SIAbstractOperation*)>;

    explicit SIAbstractOperation(QObject* parent = nullptr);
    SIAbstractOperation(const SIAbstractOperation&) = delete;
    SIAbstractOperation& operator =(const SIAbstractOperation&) = delete;
    ~SIAbstractOperation() override;

    void enqueue(const QueueFunction& queueFunction);

  signals:
    void finished(SIStatus status);

  public slots:
    void execute();
    void execute(SIDeviceAccessRegistry* deviceAccessRegistry);

  private:
    virtual void enqueue_(const QueueFunction& queueFunction);
    virtual SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) = 0;
};
