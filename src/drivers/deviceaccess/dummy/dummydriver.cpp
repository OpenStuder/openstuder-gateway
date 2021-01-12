#include "dummydriver.h"
#include <sidevice.h>
#include <sideviceaccess.h>

class DummyDevice: public SIDevice {
  public:
    DummyDevice(): SIDevice("Dummy", "0"), deviceProperties_({
            {3000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage", "Vdc"},
            {3001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature", "°C"},
            {3005, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery charge current", "Adc"},
            {3023, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Output power", "kVA"},

            {1107, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Maximum current of AC source (Input limit)", "Aac"},
            {1138, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Battery charge current", "Adc"},
            {1124, SIPropertyType::Bool, SIAccessLevel::Basic, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Inverter allowed", ""},

            {1125, SIPropertyType::Bool, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Charger allowed", ""},
            {1139, SIPropertyType::Float, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Temperature compensation", "mV/°C/cell"},
            {1206, SIPropertyType::TimeOfDay, SIAccessLevel::Expert, SIPropertyFlag::Readable | SIPropertyFlag::Writeable, "Start hour (AUX 1)", "hh:mm"},
            {1415, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "ON of the Xtenders", ""},
            {1399, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "OFF of the Xtenders", ""}
        }), data_({
            {3000, 12.0},
            {3001, 25.0},
            {3005, 1.0},
            {3023, 0.0},
            {1107, 10.0},
            {1138, 10.0},
            {1124, 1.0},
            {1125, 1.0},
            {1206, 0.0},
            {1415, {}},
            {1399, {}}
        }) {}

    const QVector<SIProperty>& properties_() const override {
        return deviceProperties_;
    }

    SIPropertyReadResult readProperty_(SIPropertyID id) const override {
        // Check if property exists and is readable.
        auto property = std::find_if(deviceProperties_.cbegin(), deviceProperties_.cend(), [&id](const SIProperty& p) { return p.id == id; });
        if (property == deviceProperties_.cend()) return {id, SIStatus::NoProperty, {}};
        if (!property->flags.testFlag(SIPropertyFlag::Readable)) return {id, SIStatus::Error, {}};

        // Return value.
        return {id, SIStatus::Success, data_.value(id)};
    }

    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) override {
        // Check if property exists and is writable.
        auto property = std::find_if(deviceProperties_.cbegin(), deviceProperties_.cend(), [&id](const SIProperty& p) { return p.id == id; });
        if (property == deviceProperties_.cend()) return {id, SIStatus::NoProperty};
        if (!property->flags.testFlag(SIPropertyFlag::Writeable)) return {id, SIStatus::Error};

        // Write value.
        data_[id] = value;
        return {id, SIStatus::Success};
    }

  private:
    QVector<SIProperty> deviceProperties_;
    QMap<SIPropertyID, QVariant> data_;
};

class DummyDeviceAccess: public SIDeviceAccess {
  public:
    DummyDeviceAccess(const QString& id): SIDeviceAccess(id) {}

    bool enumerateDevices_(QVector<SIDevice*>& devices) override {
        if (devices.isEmpty()) {
            devices.append(new DummyDevice());
        }
        return true;
    }
};

SIDeviceAccess* DummyDriver::createDeviceAccessInstance(const QString& id, const QVariantMap& parameters) {
    return new DummyDeviceAccess(id);
}
