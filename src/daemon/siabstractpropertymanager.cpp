#include "siabstractpropertymanager.h"
#include <utility>

using namespace std;

SIAbstractPropertyManager::SIAbstractPropertyManager(QObject* parent): QObject(parent) {}

SIAbstractPropertyManager::~SIAbstractPropertyManager() = default;

SIPropertyReadOperation* SIAbstractPropertyManager::readProperty(SIGlobalPropertyID id) {
    auto* operation = new SIPropertyReadOperation {move(id), this};
    enqueueOperation_(operation);
    return operation;
}

SIPropertyWriteOperation* SIAbstractPropertyManager::writeProperty(SIGlobalPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) {
    auto* operation = new SIPropertyWriteOperation {move(id), value, flags, this};
    enqueueOperation_(operation);
    return operation;
}
