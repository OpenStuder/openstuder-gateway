#pragma once
#include <QString>
#include <QVector>

class SIBluetoothProtocolFrame {
  public:
    enum Command {
        INVALID = 0x00,

        // Client messages.
        ENUMERATE = 'e',
        DESCRIBE = 'd',
        READ_PROPERTY = 'r',
        WRITE_PROPERTY = 'w',
        SUBSCRIBE_PROPERTY = 's',

        // Server messages.
        ERROR = 0xFF,
        ENUMERATED = 'E',
        DESCRIPTION = 'D',
        PROPERTY_READ = 'R',
        PROPERTY_WRITTEN = 'W',
        PROPERTY_SUBSCRIBED = 'S',
        PROPERTY_UPDATE = 'U'
    };

    explicit SIBluetoothProtocolFrame(Command command, const QVector<QString>& parameters = {});

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

    QByteArray toBytes() const;
    static SIBluetoothProtocolFrame fromBytes(const QByteArray& bytes);

  private:
    SIBluetoothProtocolFrame();

    Command command_;
    QVector<QString> parameters_;
};
