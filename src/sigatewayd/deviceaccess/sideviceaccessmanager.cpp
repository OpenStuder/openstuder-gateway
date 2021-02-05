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

    inline bool hasSubscribers() const {
        return !subscriptions_.isEmpty();
    }

    inline void addSubscriber(SIDeviceAccessManager::PropertySubscriber* subscriber) {
        subscriptions_.insert(subscriber);
    }

    inline void removeSubscriber(SIDeviceAccessManager::PropertySubscriber* subscriber) {
        subscriptions_.remove(subscriber);
    }

    inline bool isPending() const {
        return pending_;
    }

    inline void setPending() {
        pending_ = true;
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

        pending_ = false;

        return SIStatus::Success;
    }

    SIGlobalPropertyID id_;
    QSet<SIDeviceAccessManager::PropertySubscriber*> subscriptions_;
    bool pending_ = false;
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
                emit manager_->deviceMessageReceived(message);
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
    delete priv_->messageRetrieveOperation_;
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

    auto subscription = std::find_if(priv_->subscriptions_.begin(), priv_->subscriptions_.end(), [&id](SIPropertySubscriptions* sub) {
        return sub->id() == id;
    });
    if (subscription != priv_->subscriptions_.end()) {
        (*subscription)->addSubscriber(subscriber);
    } else {
        auto* newSubscription = new SIPropertySubscriptions(id);
        newSubscription->addSubscriber(subscriber);
        priv_->subscriptions_.append(newSubscription);
    }

    return true;
}

bool SIDeviceAccessManager::unsubscribeFromProperty(SIGlobalPropertyID id, SIDeviceAccessManager::PropertySubscriber* subscriber) {
    auto subscription = std::find_if(priv_->subscriptions_.begin(), priv_->subscriptions_.end(), [&id](SIPropertySubscriptions* sub) {
        return sub->id() == id;
    });
    if (subscription != priv_->subscriptions_.end()) {
        (*subscription)->removeSubscriber(subscriber);
        return true;
    } else {
        return false;
    }
}

void SIDeviceAccessManager::unsubscribeFromAllProperties(SIDeviceAccessManager::PropertySubscriber* subscriber) {
    for (auto subscription: priv_->subscriptions_) {
        subscription->removeSubscriber(subscriber);
    }
}

void SIDeviceAccessManager::startPropertyPolling(int intervalMS) {
    startTimer(intervalMS);
}

void SIDeviceAccessManager::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event)

    // Remove unused subscriptions.
    for (auto i = priv_->subscriptions_.begin(); i != priv_->subscriptions_.end();) {
        if ((*i)->isPending() ||( *i)->hasSubscribers()) {
            ++i;
        } else {
            delete *i;
            i = priv_->subscriptions_.erase(i);
        }
    }

    // Enqueue device message retrieve operation.
    enqueueOperation_(priv_->messageRetrieveOperation_);

    // Enqueue all subscriptions that are not pending.
    for (auto* subscription: priv_->subscriptions_) {
        if (!subscription->isPending()) {
            subscription->setPending();
            enqueueOperation_(subscription);
        }
    }
}
