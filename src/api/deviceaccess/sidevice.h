#pragma once
#include "siproperty.h"
#include "sipropertyreadresult.h"
#include "sipropertywriteflags.h"
#include "sipropertywriteresult.h"
#include "siaccesslevel.h"
#include "sijdescriptionflags.h"
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

    QJsonObject jsonDescription(SIAccessLevel accessLevel, SIDescriptionFlags flags) const;

    SIProperty property(SIPropertyID id) const;

    SIPropertyReadResult readProperty(SIPropertyID id) const;
    QVector<SIPropertyReadResult> readProperties(const QVector<SIPropertyID>& ids) const;

    SIPropertyWriteResult writeProperty(SIPropertyID id, const QVariant& value = {}, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default);
    QVector<SIPropertyWriteResult> writeProperties(const QVector<const QPair<SIPropertyID,const QVariant>>& properties, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default);

  protected:
    explicit SIDevice(const QString& model, const QString& id);

  private:
    virtual const QVector<SIProperty>& properties_() const = 0;

    virtual SIPropertyReadResult readProperty_(SIPropertyID id) const = 0;
    virtual QVector<SIPropertyReadResult> readProperties_(const QVector<SIPropertyID>& ids) const;

    virtual SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) = 0;
    virtual QVector<SIPropertyWriteResult> writeProperties_(const QVector<const QPair<SIPropertyID,const QVariant>>& properties, SIPropertyWriteFlags flags);

    virtual void completeJsonDescription_(QJsonObject& object, SIDescriptionFlags flags) const;

    struct Private_;
    std::unique_ptr<Private_> private_;
};
