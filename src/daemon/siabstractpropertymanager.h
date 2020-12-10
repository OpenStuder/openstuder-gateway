#pragma once
#include <sipropertyid.h>
#include <sipropertyreadresult.h>
#include <sipropertywriteresult.h>
#include <QObject>
#include <sipropertywriteflags.h>
#include "sipropertyreadoperation.h"
#include "sipropertywriteoperation.h"


class SIAbstractPropertyManager: public QObject {
    Q_OBJECT

  public:
    explicit SIAbstractPropertyManager(QObject* parent = nullptr);
    ~SIAbstractPropertyManager() override;


    SIPropertyReadOperation* readProperty(SIGlobalPropertyID id);
    SIPropertyWriteOperation* writeProperty(SIGlobalPropertyID id, const QVariant& value, SIPropertyWriteFlags flags = SIPropertyWriteFlag::Default);

  private:
    virtual void enqueueOperation_(SIAbstractOperation* operation) = 0;
};
