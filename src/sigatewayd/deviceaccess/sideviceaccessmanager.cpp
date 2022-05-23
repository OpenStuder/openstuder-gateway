#include "sideviceaccessmanager.h"
#include "sideviceaccessregistry.h"
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
        if (result.status() == SIStatus::Success) {
            for (auto* subscriber: subscriptions_) {
                subscriber->propertyChanged(id_, result.value());
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
        for (int i = 0; i < deviceAccessCount; ++i) {
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
    SIMessagesRetrieveOperation* messageRetrieveOperation_ = nullptr;
};

SIDeviceAccessManager::SIDeviceAccessManager(QObject* parent): QObject(parent), priv_(new Private) {
    priv_->messageRetrieveOperation_ = new SIMessagesRetrieveOperation(this);

    for (int i = 0; i < SIDeviceAccessRegistry::sharedRegistry().deviceAccessCount(); ++i) {
        connect(SIDeviceAccessRegistry::sharedRegistry().deviceAccess(i), &SIDeviceAccess::deviceAdded, this, &SIDeviceAccessManager::onDeviceAdded_);
        connect(SIDeviceAccessRegistry::sharedRegistry().deviceAccess(i), &SIDeviceAccess::deviceRemoved, this, &SIDeviceAccessManager::onDeviceRemoved_);
    }
}

SIDeviceAccessManager::~SIDeviceAccessManager() {
    qDeleteAll(priv_->subscriptions_);
    delete priv_->messageRetrieveOperation_;
    delete priv_;
}

SIProperty SIDeviceAccessManager::resolveProperty(const SIGlobalPropertyID& id) {
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

QVector<SIGlobalPropertyID> SIDeviceAccessManager::findProperties(SIGlobalPropertyID propertyID, bool virtualDevices, SIDeviceFunctions functionsMask) {
    QVector<SIGlobalPropertyID> ids;

    if (propertyID.isWildcard()) {
        for (int ai = 0; ai < SIDeviceAccessRegistry::sharedRegistry().deviceAccessCount(); ++ai) {
            const auto deviceAccess = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(ai);

            if (propertyID.accessID() == "*" || propertyID.accessID() == deviceAccess->id()) {
                for (int di = 0; di < deviceAccess->deviceCount(); ++di) {
                    const auto device = deviceAccess->device(di);
                    if (virtualDevices == device->isVirtual() && functionsMask & device->functions()) {
                        if (propertyID.deviceID() == "*" || propertyID.deviceID() == device->id()) {
                            if (device->property(propertyID.propertyID()).type() != SIPropertyType::Invalid) {
                                ids.append({deviceAccess->id(), device->id(), propertyID.propertyID()});
                            }
                        }
                    }
                }
            }
        }
    }

    return ids;
}

SIDeviceEnumerationOperation* SIDeviceAccessManager::enumerateDevices() {
    auto* operation = new SIDeviceEnumerationOperation(this);
    operation->enqueue(std::bind(&SIDeviceAccessManager::enqueueOperation_, this, std::placeholders::_1));
    return operation;
}

SIPropertyReadOperation* SIDeviceAccessManager::readProperty(const SIGlobalPropertyID& id) {
    auto* operation = new SIPropertyReadOperation {id, this};
    operation->enqueue(std::bind(&SIDeviceAccessManager::enqueueOperation_, this, std::placeholders::_1));
    return operation;
}

SIPropertiesReadOperation* SIDeviceAccessManager::readProperties(const QVector<SIGlobalPropertyID>& ids) {
    auto* operation = new SIPropertiesReadOperation(ids, this);
    operation->enqueue(std::bind(&SIDeviceAccessManager::enqueueOperation_, this, std::placeholders::_1));
    return operation;
}

SIPropertyWriteOperation* SIDeviceAccessManager::writeProperty(const SIGlobalPropertyID& id, const QVariant& value, SIPropertyWriteFlags flags) {
    auto* operation = new SIPropertyWriteOperation {id, value, flags, this};
    operation->enqueue(std::bind(&SIDeviceAccessManager::enqueueOperation_, this, std::placeholders::_1));
    return operation;
}

bool SIDeviceAccessManager::subscribeToProperty(const SIGlobalPropertyID& id, SIDeviceAccessManager::PropertySubscriber* subscriber) {
    if (resolveProperty(id).type() == SIPropertyType::Invalid) {
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

bool SIDeviceAccessManager::unsubscribeFromProperty(const SIGlobalPropertyID& id, SIDeviceAccessManager::PropertySubscriber* subscriber) {
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

void SIDeviceAccessManager::onDeviceAdded_(const SIDevice& device) {
    auto* access = reinterpret_cast<SIDeviceAccess*>(sender());
    emit deviceAdded(*access, device);
}

void SIDeviceAccessManager::onDeviceRemoved_(const SIDevice& device) {
    auto* access = reinterpret_cast<SIDeviceAccess*>(sender());
    emit deviceRemoved(*access, device);
}

void SIDeviceAccessManager::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event)

    // Remove unused subscriptions.
    for (auto i = priv_->subscriptions_.begin(); i != priv_->subscriptions_.end();) {
        if ((*i)->isPending() || (*i)->hasSubscribers()) {
            ++i;
        } else {
            delete *i;
            i = priv_->subscriptions_.erase(i);
        }
    }

    // Enqueue device message retrieve operation.
    priv_->messageRetrieveOperation_->enqueue(std::bind(&SIDeviceAccessManager::enqueueOperation_, this, std::placeholders::_1));

    // Enqueue all subscriptions that are not pending.
    for (auto* subscription: priv_->subscriptions_) {
        if (!subscription->isPending()) {
            subscription->setPending();
            subscription->enqueue(std::bind(&SIDeviceAccessManager::enqueueOperation_, this, std::placeholders::_1));
        }
    }
}
