#pragma once
#include "siproperty.h"
#include "sipropertyreadresult.h"
#include "sipropertywriteresult.h"
#include "sijsonflags.h"
#include <memory>
#include <QObject>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QPair>
#include <QJsonObject>

class SIDeviceAccess;

class SIDevice: public QObject {
    Q_OBJECT

  public:
    SIDevice(const SIDevice&) = delete;
    SIDevice& operator =(const SIDevice&) = delete;
    ~SIDevice() override;

    const QString& model() const;

    const QString& id() const;

    const QVector<SIProperty>& properties() const;

    const QJsonObject& jsonDescription(SIJsonFlags flags) const;

    SIPropertyReadResult readProperty(SIPropertyID id) const;
    QVector<SIPropertyReadResult> readProperties(const QVector<SIPropertyID>& ids) const;

    SIPropertyWriteResult writeProperty(SIPropertyID id, const QVariant& value);
    QVector<SIPropertyWriteResult> writeProperties(const QVector<const QPair<SIPropertyID,const QVariant>>& properties);

  protected:
    explicit SIDevice(const QString& model, const QString& id);

  private:
    virtual const QVector<SIProperty>& properties_() const = 0;

    virtual SIPropertyReadResult readProperty_(SIPropertyID id) const = 0;
    virtual QVector<SIPropertyReadResult> readProperties_(const QVector<SIPropertyID>& ids) const;

    virtual SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value) = 0;
    virtual QVector<SIPropertyWriteResult> writeProperties_(const QVector<const QPair<SIPropertyID,const QVariant>>& properties);

    virtual void completeJsonDescription_(QJsonObject& object, SIJsonFlags flags) const;

    struct Private_;
    std::unique_ptr<Private_> priv_;
};
