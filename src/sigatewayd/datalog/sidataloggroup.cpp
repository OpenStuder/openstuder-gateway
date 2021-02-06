#include "sidataloggroup.h"
#include "../deviceaccess/sideviceaccessmanager.h"
#include "../deviceaccess/operations/sipropertiesreadoperation.h"
#include "../deviceaccess/operations/sipropertyreadoperation.h"
#include <sistorage.h>
#include <QDateTime>

#include <QtDebug>

SIDataLogGroup::SIDataLogGroup(int interval, const QVector<SIGlobalPropertyID>& propertyIDs, SIContext* context, QObject* parent):
    QObject(parent), interval_(interval), propertyIDs_(propertyIDs), context_(context) {
    timer_.setSingleShot(true);
    connect(&timer_, &QTimer::timeout, this, &SIDataLogGroup::onTimeout_);
}

void SIDataLogGroup::startPropertyPolling() {
    if (!active_) {
        active_ = true;
        timer_.start(msecToNextDue_());
    }
}

void SIDataLogGroup::stopPropertyPolling() {
    if (active_) {
        active_ = false;
        timer_.stop();
    }
}

void SIDataLogGroup::onTimeout_() {
    if (active_) {
        qDebug() << "START READ";
        auto* operation = context_->deviceAccessManager().readProperties(propertyIDs_);
        connect(operation, &SIAbstractOperation::finished, this, &SIDataLogGroup::onFinished_);
    }
}

void SIDataLogGroup::onFinished_(SIStatus status) {
    Q_UNUSED(status)

    qDebug() << "READ COMPLETE";

    auto* operations = dynamic_cast<SIPropertiesReadOperation*>(sender());
    QMap<SIGlobalPropertyID, QVariant> results;
    for (int i = 0; i < operations->count(); ++i) {
        auto& op = (*operations)[i];
        if (op.status() == SIStatus::Success) {
            results[op.id()] = op.value();
        }
    }
    context_->storage().storePropertyValues(results);
    delete operations;
    if (active_) {
        timer_.start(msecToNextDue_());
    }
}

int SIDataLogGroup::msecToNextDue_() const {
    return (interval_ * 1000) - QDateTime::currentMSecsSinceEpoch() % (interval_ * 1000);
}
