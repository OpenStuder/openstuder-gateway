#pragma once
#include "sipropertyhistorystorage.h"
#include "sidevicemessagestorage.h"

class SIStorage: public SIPropertyHistoryStorage, public SIDeviceMessageStorage {};
