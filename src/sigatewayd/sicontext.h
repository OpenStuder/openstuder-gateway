#pragma once
class SISettings;
class SIDeviceAccessManager;
class SIUserAuthorizer;
class SIStorage;

class SIContext {
  public:
    ~SIContext() = default;

    virtual const SISettings& settings() const = 0;
    virtual SIDeviceAccessManager& deviceAccessManager() = 0;
    virtual const SIUserAuthorizer* userAuthorizer() = 0;
    virtual SIStorage& storage() = 0;
};
