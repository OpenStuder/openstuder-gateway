#include "sidataloggroup.h"
#include "../deviceaccess/sideviceaccessmanager.h"
#include "../deviceaccess/operations/sipropertiesreadoperation.h"
#include "../deviceaccess/operations/sipropertyreadoperation.h"
#include <sistorage.h>
#include <QDateTime>

SIDataLogGroup::SIDataLogGroup(int interval, const QVector<SIGlobalPropertyID>& propertyIDs, SIContext* context, QObject* parent):
    QObject(parent), interval_(interval), context_(context) {
    for (const auto& id: propertyIDs) {
        if (id.isWildcard()) {
            wildcardIDs_.append(id);
        } else {
            propertyIDs_.append(id);
        }
    }

    timer_.setSingleShot(true);
    timer_.setTimerType(Qt::PreciseTimer);
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

void SIDataLogGroup::addWildcardPropertiesForDevice(const QString& accessID, const QString& deviceID, const QVector<SIProperty>& properties) {
    for (const auto& wildcardID: wildcardIDs_) {
        for (const auto& property: properties) {
            if (wildcardID.matches(accessID, deviceID, property.id)) {
                propertyIDs_.append({accessID, deviceID, property.id});
            }
        }
    }
}

void SIDataLogGroup::removeWildcardPropertiesForDevice(const QString& accessID, const QString& deviceID) {
    propertyIDs_.erase(std::remove_if(propertyIDs_.begin(), propertyIDs_.end(), [&accessID, &deviceID](const SIGlobalPropertyID& id) {
        return id.accessID() == accessID && id.deviceID() == deviceID;
    }), propertyIDs_.end());
}

void SIDataLogGroup::onTimeout_() {
    if (active_) {
        if (!propertyIDs_.isEmpty()) {
            auto* operation = context_->deviceAccessManager().readProperties(propertyIDs_);
            connect(operation, &SIAbstractOperation::finished, this, &SIDataLogGroup::onFinished_);
        } else {
            timer_.start(msecToNextDue_());
        }
    }
}

void SIDataLogGroup::onFinished_(SIStatus status) {
    Q_UNUSED(status)

    auto* operation = dynamic_cast<SIPropertiesReadOperation*>(sender());
    QMap<SIGlobalPropertyID, QVariant> results;
    for (int i = 0; i < operation->count(); ++i) {
        auto& op = (*operation)[i];
        if (op.status() == SIStatus::Success) {
            results[op.id()] = op.value();
        } else {
            results[op.id()] = {};
        }
    }
    context_->storage().storePropertyValues(results);
    operation->deleteLater();
    if (active_) {
        timer_.start(msecToNextDue_());
    }
}

int SIDataLogGroup::msecToNextDue_() const {
    auto msecToNextDue = (interval_ * 1000) - QDateTime::currentMSecsSinceEpoch() % (interval_ * 1000);
    if (msecToNextDue < 500) {
        msecToNextDue += interval_;
    }
    return msecToNextDue;
}
