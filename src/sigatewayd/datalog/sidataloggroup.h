#pragma once
#include <QObject>
#include "../deviceaccess/siglobalpropertyid.h"
#include "../sicontext.h"
#include <sistatus.h>
#include <siproperty.h>
#include <QVector>
#include <QTimer>

class SIDataLogGroup: public QObject {
  Q_OBJECT

  public:
    explicit SIDataLogGroup(int interval, const QVector<SIGlobalPropertyID>& propertyIDs, SIContext* context, QObject* parent = nullptr);
    SIDataLogGroup(const SIDataLogGroup&) = delete;
    SIDataLogGroup& operator =(const SIDataLogGroup&) = delete;

    void startPropertyPolling();
    void stopPropertyPolling();

    void addWildcardPropertiesForDevice(const QString& accessID, const QString& deviceID, const QVector<SIProperty>& properties);
    void removeWildcardPropertiesForDevice(const QString& accessID, const QString& deviceID);

  private slots:
    void onTimeout_();
    void onFinished_(SIStatus status);

  private:
    qint64 msecToNextDue_() const;

    int interval_;
    bool active_ = false;
    QVector<SIGlobalPropertyID> propertyIDs_;
    QVector<SIGlobalPropertyID> wildcardIDs_;
    SIContext* context_;
    QTimer timer_;
};
