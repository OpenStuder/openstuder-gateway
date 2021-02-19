#pragma once
#include "sipropertyhistorystorage.h"
#include "sidevicemessagestorage.h"

class SIStorage: public SIPropertyHistoryStorage, public SIDeviceMessageStorage {
  public:
    SIStorage() = default;
    SIStorage(const SIStorage&) = delete;
    SIStorage& operator =(const SIStorage&) = delete;
    ~SIStorage() override = default;
};
