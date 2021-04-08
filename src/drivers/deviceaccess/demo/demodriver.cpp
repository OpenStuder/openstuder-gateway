#include "demodriver.h"
#include <sidevice.h>
#include <sideviceaccess.h>
#include <QElapsedTimer>
#include <QTime>
#include <random>

class DemoModel: QObject {
  public:
    DemoModel() = default;

    void start(int updateIntervalMS) {
        if (timerID_ == -1) {
            timerID_ = startTimer(updateIntervalMS);
            elapsed_.start();
        }
    }

    void stop() {
        if (timerID_ != -1) {
            killTimer(timerID_);
            timerID_ = -1;
        }
    }

    inline double solarPower() const {
        return solarPower_;
    }

    inline double acInputPower() const {
        return acInputPower_;
    }

    inline bool acOutputOn() const {
        return acOutputOn_;
    }

    inline void setAcOutputOn(bool on) {
        acOutputOn_ = on;
    }

    inline double acOutputPower() const {
        return acOutputPower_;
    }

    inline double batteryChargePower() const {
        return batteryChargePower_;
    }

    inline double batteryVoltage() const {
        return 11.8 + 1.7 * batteryStateOfCharge();
    }

    inline double batteryChargeCurrent() const {
        return batteryChargePower_ / batteryVoltage();
    }

    inline double batteryStateOfCharge() const {
        return batteryCharge_ / batteryCapacity_;
    }

    inline double batteryTemperature() const {
        return batteryTemperature_;
    }

    inline void addMessage(const SIDeviceMessage& message) {
        pendingMessages_.append(message);
    }

    inline const QVector<SIDeviceMessage>& pendingMessages() const {
        return pendingMessages_;
    }

    inline void clearPendingMessages() {
        pendingMessages_.clear();
    }

  private:
    static double solarRadiance_(double minuteOfDay) {
        return (minuteOfDay > 300. && minuteOfDay < 1100.) ? std::max(0., 800. - pow(0.1 * (minuteOfDay - 700.), 2) + 0.0003 * pow(0.1 * (minuteOfDay - 700.), 4)) : 0.;
    }

    static double powerConsumption_(double minuteOfDay) {
        double factor = 0;
        if (minuteOfDay > 400.) { factor = 200; }
        if (minuteOfDay > 1000.) { factor = 25; }
        if (minuteOfDay > 1400.) { factor = 0; }
        std::mt19937 random_engine; // NOLINT(cert-msc51-cpp)
        random_engine.seed(static_cast<unsigned int>(minuteOfDay / 12.0));
        return factor * 1. + factor * 0.5 * static_cast<double>(random_engine()) / std::mt19937::max();
    }

    void update_(double deltaTime, double minuteOfDay = static_cast<double>(QTime::currentTime().msecsSinceStartOfDay() / 1000 / 60)) {
        acOutputOn_ = acOutputOn_ && batteryCharge_ > 0.05;
        acInputOn_ = batteryCharge_ < (acInputOn_ ? 0.95 * batteryCapacity_ : 0.1 * batteryCapacity_);
        auto batteryFull = batteryCharge_ >= batteryCapacity_;

        solarPower_ = solarRadiance_(minuteOfDay) * solarPanelSurface_ * solarEfficiency_;
        acOutputPower_ = acOutputOn_ ? powerConsumption_(minuteOfDay) : 0.;
        acInputPower_ = acInputOn_ ? acChargePower_ : 0.;

        if (batteryFull) {
            auto remainingOutPower = acOutputPower_;
            remainingOutPower -= solarPower_;
            if (remainingOutPower <= 0) {
                solarPower_ += remainingOutPower;
                acInputPower_ = 0.;
                batteryChargePower_ = 0;
            } else {
                remainingOutPower -= acInputPower_;
                if (remainingOutPower <= 0) {
                    acInputPower_ += remainingOutPower;
                    batteryChargePower_ = 0;
                } else {
                    batteryChargePower_ = -remainingOutPower;
                }
            }
        } else {
            batteryChargePower_ = solarPower_ + acInputPower_ - acOutputPower_;
        }
        batteryCharge_ += deltaTime * batteryChargePower_;

        batteryTemperature_ = batteryTemperature_ - 0.1 * (batteryTemperature_ - 20) + 0.0005 * abs(batteryChargePower_);
    }

    void timerEvent(QTimerEvent* event) override {
        Q_UNUSED(event);

        update_(elapsed_.restart() / 1000.);
    }

    int timerID_ = -1;
    QElapsedTimer elapsed_;

    // Solar characteristics.
    double solarPanelSurface_ = 3.2; // m^2
    double solarEfficiency_ = 0.20;

    // AC input characteristics.
    double acChargePower_ = 1000.; // W

    // Battery characteristics.
    double batteryCapacity_ = 200 /* Ah */ * 3600 /* C/Ah */;

    // Variables.
    bool acOutputOn_ = true;
    bool acInputOn_ = false;

    double solarPower_ = 0.;         // W
    double acOutputPower_ = 0.;      // W
    double acInputPower_ = 0.;       // w
    double batteryChargePower_ = 0.; // W
    double batteryCharge_ = batteryCapacity_ / 2.; // C
    double batteryTemperature_ = 20; // degree celsius

    QVector<SIDeviceMessage> pendingMessages_;
};

class DemoInverter: public SIDevice {
  public:
    explicit DemoInverter(const QString& id, DemoModel& model):
        SIDevice("Demo inverter", id),
        deviceProperties_({
                              {3032, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of auxiliary relay 2", {
                                  {0, "Opened"},
                                  {1, "Closed"}
                              }},
                              {3137, SIPropertyType::Float,  SIAccessLevel::Basic,  SIPropertyFlag::Readable,  "Input active power",                   "kW"},
                              {3136, SIPropertyType::Float,  SIAccessLevel::Basic,  SIPropertyFlag::Readable,  "Output active power",                  "kW"},
                              {3049, SIPropertyType::Enum, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of the inverter", {
                                                                                                                                            {0, "Off"},
                                                                                                                                            {1, "On"}
                                                                                                                                        }},
                              {3081, SIPropertyType::Float,  SIAccessLevel::Basic,  SIPropertyFlag::Readable,  "Energy AC-In from the current day",    "kWh"},
                              {3080, SIPropertyType::Float,  SIAccessLevel::Basic,  SIPropertyFlag::Readable,  "Energy AC-In from the previous day",   "kWh"},
                              {3083, SIPropertyType::Float,  SIAccessLevel::Basic,  SIPropertyFlag::Readable,  "Consumers energy of the current day",  "kWh"},
                              {3082, SIPropertyType::Float,  SIAccessLevel::Basic,  SIPropertyFlag::Readable,  "Consumers energy of the previous day", "kWh"},
                              {1415, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "ON of the Xtenders",                   ""},
                              {1399, SIPropertyType::Signal, SIAccessLevel::Expert, SIPropertyFlag::Writeable, "OFF of the Xtenders",                  ""}
                          }), model_(model) {}

    const QVector<SIProperty>& properties_() const override {
        return deviceProperties_;
    }

  private:
    SIPropertyReadResult readProperty_(SIPropertyID id) const override {
        switch (id) {
            case 3032:
                return {3137, SIStatus::Success, model_.acOutputOn() ? 1 : 0};

            case 3137:
                return {3137, SIStatus::Success, model_.acInputPower() / 1000.};

            case 3136:
                return {3136, SIStatus::Success, model_.acOutputPower() / 1000.};

            case 3049:
                return {3049, SIStatus::Success, model_.acOutputOn() ? 1.0 : 0.0};

            case 3081:
            case 3080:
            case 3083:
            case 3082:
                return {id, SIStatus::Success, id};

            default:
                return {id, SIStatus::NoProperty, {}};
        }
    }

    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) override {
        Q_UNUSED(flags)
        Q_UNUSED(value)

        switch (id) {
            case 1415:
            case 1399: {
                auto on = id == 1415;
                if (model_.acOutputOn() != on) {
                    if (on) {
                        model_.addMessage({"", this->id(), 209, "AUX2 relay activation"});
                    } else {
                        model_.addMessage({"", this->id(), 210, "AUX2 relay deactivation"});
                    }
                }
                model_.setAcOutputOn(on);
                return {id, SIStatus::Success};
            }

            default:
                return {id, SIStatus::NoProperty};
        }
    }

    QVector<SIProperty> deviceProperties_;
    DemoModel& model_;
};

class DemoMPPT: public SIDevice {
  public:
    explicit DemoMPPT(const QString& id, DemoModel& model):
        SIDevice("Demo MPPT", id),
        deviceProperties_({
                              {11004, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Power of the PV generator",                "kW"},
                              {11007, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production in (kWh) for the current day",  "kWh"},
                              {11011, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Production in (kWh) for the previous day", "kWh"}
                          }), model_(model) {}

  private:
    const QVector<SIProperty>& properties_() const override {
        return deviceProperties_;
    }

    SIPropertyReadResult readProperty_(SIPropertyID id) const override {
        switch (id) {
            case 11004:
                return {11004, SIStatus::Success, model_.solarPower() / 1000.};

            case 11007:
            case 11011:
                return {id, SIStatus::Success, id};

            default:
                return {id, SIStatus::NoProperty, {}};
        }
    }

    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) override {
        Q_UNUSED(value)
        Q_UNUSED(flags)

        return {id, SIStatus::NoProperty};
    }

    QVector<SIProperty> deviceProperties_;
    DemoModel& model_;
};

class DemoBSP: public SIDevice {
  public:
    explicit DemoBSP(const QString& id, DemoModel& model):
        SIDevice("Demo BSP", id),
        deviceProperties_({
                              {7003, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Power",                            "W"},
                              {7000, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery voltage",                  "V"},
                              {7001, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery current",                  "A"},
                              {7002, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "State of Charge",                  "%"},
                              {7033, SIPropertyType::Float, SIAccessLevel::Basic, SIPropertyFlag::Readable, "Battery temperature (minute avg)", "degree C"}
                          }), model_(model) {}

  private:
    const QVector<SIProperty>& properties_() const override {
        return deviceProperties_;
    }

    SIPropertyReadResult readProperty_(SIPropertyID id) const override {
        switch (id) {
            case 7003:
                return {7003, SIStatus::Success, model_.batteryChargePower()};

            case 7000:
                return {7000, SIStatus::Success, model_.batteryVoltage()};

            case 7001:
                return {7001, SIStatus::Success, model_.batteryChargeCurrent()};

            case 7002:
                return {7002, SIStatus::Success, model_.batteryStateOfCharge()};

            case 7033:
                return {7033, SIStatus::Success, model_.batteryTemperature()};

            default:
                return {id, SIStatus::NoProperty, {}};
        }
    }

    SIPropertyWriteResult writeProperty_(SIPropertyID id, const QVariant& value, SIPropertyWriteFlags flags) override {
        Q_UNUSED(value)
        Q_UNUSED(flags)

        return {id, SIStatus::NoProperty};
    }

    QVector<SIProperty> deviceProperties_;
    DemoModel& model_;
};

class DemoDeviceAccess: public SIDeviceAccess {
  public:
    explicit DemoDeviceAccess(const QString& id): SIDeviceAccess(id) {
        model_.start(2500);
    }

    bool enumerateDevices_(QVector<SIDevice*>& devices) override {
        if (devices.isEmpty()) {
            devices.append(new DemoInverter("inv", model_));
            devices.append(new DemoMPPT("sol", model_));
            devices.append(new DemoBSP("bat", model_));
        }
        return true;
    }

  private:
    void retrievePendingDeviceMessages_(QVector<SIDeviceMessage>& messages) const override {
        auto msgs = model_.pendingMessages();
        model_.clearPendingMessages();
        for (auto& msg: msgs) msg.setAccessID(id());
        messages.append(msgs);
    }

  private:
    mutable DemoModel model_;
};

SIDeviceAccess* DemoDriver::createDeviceAccessInstance(const QString& id, const QVariantMap& parameters) {
    Q_UNUSED(parameters)

    return new DemoDeviceAccess(id);
}
