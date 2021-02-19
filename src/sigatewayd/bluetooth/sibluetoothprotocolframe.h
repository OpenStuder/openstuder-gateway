#pragma once
#include <QString>
#include <QVector>

class SIBluetoothProtocolFrame {
  public:
    enum Command {
        INVALID = 0x00,

        // Client messages.
        AUTHORIZE = 'a',
        ENUMERATE = 'e',
        READ_PROPERTY = 'r',
        WRITE_PROPERTY = 'w',
        SUBSCRIBE_PROPERTY = 's',

        // Server messages.
        ERROR = 0xFF,
        AUTHORIZED = 'A',
        ENUMERATED = 'E',
        PROPERTY_READ = 'R',
        PROPERTY_WRITTEN = 'W',
        PROPERTY_SUBSCRIBED = 'S',
        PROPERTY_UPDATE = 'U',
        DEVICE_MESSAGE = 'M'
    };

    SIBluetoothProtocolFrame(Command command = INVALID, const QVector<QString>& parameters = {}); // NOLINT(google-explicit-constructor)

    bool isNull() const;

    inline Command command() const {
        return command_;
    }

    inline void setCommand(Command command) {
        command_ = command;
    }

    inline const QVector<QString>& parameters() const {
        return parameters_;
    }

    inline void setParameters(const QVector<QString>& parameters) {
        parameters_ = parameters;
    }

    inline int parameterCount() const {
        return parameters_.count();
    }

    inline bool isParameterCountInRange(int min, int max) const {
        return parameters_.count() <= min && parameters_.count() >= max;
    }

    QByteArray toBytes() const;
    static SIBluetoothProtocolFrame fromBytes(const QByteArray& bytes);

  private:
    Command command_;
    QVector<QString> parameters_;
};
