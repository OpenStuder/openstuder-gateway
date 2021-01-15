#include "sideviceaccessmanager.h"
#include "sideviceaccessregistry.h"
#include "sideviceaccess.h"
#include "sidevice.h"
#include <utility>
#include <QSet>
#include <QtDebug>

using namespace std;

class SIPropertySubscriptions final: public SIAbstractOperation {
  public:
    inline explicit SIPropertySubscriptions(SIGlobalPropertyID id, QObject* parent = nullptr): SIAbstractOperation(parent), id_(move(id)) {}

    inline const SIGlobalPropertyID& id() const {
        return id_;
    }

    inline QSet<SIDeviceAccessManager::PropertySubscriber*>& subscriptions() {
        return subscriptions_;
    }

  private:
    SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override {
        if (deviceAccessRegistry == nullptr) {
            return SIStatus::Error;
        }

        auto deviceAccess = deviceAccessRegistry->deviceAccess(id_.accessID());
        if (deviceAccess == nullptr) {
            return SIStatus::NoDeviceAccess;
        }

        auto device = deviceAccess->device(id_.deviceID());
        if (device == nullptr) {
            return SIStatus::NoDevice;
        }

        auto result = device->readProperty(id_.propertyID());
        if (result.status == SIStatus::Success) {
           for (auto* subscriber: subscriptions_) {
               subscriber->propertyChanged(id_, result.value);
           }
        }

        return SIStatus::Success;
    }

    SIGlobalPropertyID id_;
    QSet<SIDeviceAccessManager::PropertySubscriber*> subscriptions_;
};

class SIMessagesRetrieveOperation final: public SIAbstractOperation {
  public:
    inline explicit SIMessagesRetrieveOperation(SIDeviceAccessManager* manager, QObject* parent = nullptr): SIAbstractOperation(parent), manager_(manager) {}

  private:
    SIStatus execute_(SIDeviceAccessRegistry* deviceAccessRegistry) override {
        if (deviceAccessRegistry == nullptr) {
            return SIStatus::Error;
        }

        auto deviceAccessCount = deviceAccessRegistry->deviceAccessCount();
        for (int i = 0 ; i < deviceAccessCount; ++i) {
            auto deviceAccess = deviceAccessRegistry->deviceAccess(i);
            for (const auto& message: deviceAccess->retrievePendingDeviceMessages()) {
                emit manager_->deviceMessageReceived(deviceAccess->id(), message);
            }
        }

        return SIStatus::Success;
    }

    SIDeviceAccessManager* manager_;
};

struct SIDeviceAccessManager::Private {
    QVector<SIPropertySubscriptions*> subscriptions_;
    SIMessagesRetrieveOperation* messageRetrieveOperation_;
};

SIDeviceAccessManager::SIDeviceAccessManager(QObject* parent): QObject(parent), priv_(new Private) {
    priv_->messageRetrieveOperation_ = new SIMessagesRetrieveOperation(this);
}

SIDeviceAccessManager::~SIDeviceAccessManager() {
    qDeleteAll(priv_->subscriptions_);
    delete priv_;
}

SIProperty SIDeviceAccessManager::resolveProperty(SIGlobalPropertyID id) {
    if (!id.isValid()) {
        return {};
    }

    auto deviceAccess = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(id.accessID());
    if (deviceAccess == nullptr) {
        return {};
    }

    auto device = deviceAccess->device(id.deviceID());
    if (device == nullptr) {
        return {};
    }

    return device->property(id.propertyID());
}

SIDeviceEnumerationOperation* SIDeviceAccessManager::enumerateDevices() {
    auto* operation = new SIDeviceEnumerationOperation(this);
    enqueueOperation_(operation);
    return operation;
}

SIPropertyReadOperation* SIDeviceAccessManager::readProperty(SIGlobalPropertyID id) {
    auto* operation = new SIPropertyReadOperation {move(id), this};
    enqueueOperation_(operation);
    return operation;
}

SIPropertyWriteOperation* SIDeviceAccessManager::writeProperty(SIGlobalPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) {
    auto* operation = new SIPropertyWriteOperation {move(id), value, flags, this};
    enqueueOperation_(operation);
    return operation;
}

bool SIDeviceAccessManager::subscribeToProperty(SIGlobalPropertyID id, SIDeviceAccessManager::PropertySubscriber* subscriber) {
    if (resolveProperty(id).type == SIPropertyType::Invalid) {
        return false;
    }

    auto subscription = std::find_if(priv_->subscriptions_.begin(), priv_->subscriptions_.end(), [&id](SIPropertySubscriptions* pollOperation) {
        return pollOperation->id() == id;
    });
    if (subscription != priv_->subscriptions_.end()) {
        (*subscription)->subscriptions().insert(subscriber);
    } else {
        auto* subscription = new SIPropertySubscriptions(id);
        subscription->subscriptions().insert(subscriber);
        priv_->subscriptions_.append(subscription);
    }

    return true;
}

bool SIDeviceAccessManager::unsubscribeFromProperty(SIGlobalPropertyID id, SIDeviceAccessManager::PropertySubscriber* subscriber) {
    auto subscription = std::find_if(priv_->subscriptions_.begin(), priv_->subscriptions_.end(), [&id](SIPropertySubscriptions* pollOperation) {
        return pollOperation->id() == id;
    });
    if (subscription != priv_->subscriptions_.end()) {
        (*subscription)->subscriptions().remove(subscriber);
        if ((*subscription)->subscriptions().isEmpty()) {
            delete *priv_->subscriptions_.erase(subscription);
        }
        return true;
    } else {
        return false;
    }
}

void SIDeviceAccessManager::unsubscribeFromAllProperties(SIDeviceAccessManager::PropertySubscriber* subscriber) {
    for (auto i = priv_->subscriptions_.begin(); i != priv_->subscriptions_.end();) {
        if ((*i)->subscriptions().remove(subscriber) && (*i)->subscriptions().isEmpty()) {
            delete *i;
            i = priv_->subscriptions_.erase(i);
        } else {
            ++i;
        }
    }
}

void SIDeviceAccessManager::startPropertyPolling(int intervalMS) {
    startTimer(intervalMS);
}

void SIDeviceAccessManager::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event)

    enqueueOperation_(priv_->messageRetrieveOperation_);
    for (auto* subscription: priv_->subscriptions_) {
        enqueueOperation_(subscription);
    }
}
