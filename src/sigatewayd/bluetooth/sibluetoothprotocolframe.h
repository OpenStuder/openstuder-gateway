#pragma once
#include <QString>
#include <QVector>
#include <QSet>
#include <QVariant>

struct CborEncoder;

class SIBluetoothProtocolFrame {
  public:
    enum Command {
        INVALID = 0x00,

        // Client messages.
        AUTHORIZE = 0x01,
        ENUMERATE = 0x02,
        DESCRIBE = 0x03,
        READ_PROPERTY = 0x04,
        WRITE_PROPERTY =0x05,
        SUBSCRIBE_PROPERTY = 0x06,
        UNSUBSCRIBE_PROPERTY = 0x07,
        READ_DATALOG = 0x08,
        READ_MESSAGES = 0x09,
        FIND_PROPERTIES = 0x0A,

        // Server messages.
        ERROR = 0xFF,
        AUTHORIZED = 0x81,
        ENUMERATED = 0x82,
        DESCRIPTION = 0x83,
        PROPERTY_READ = 0x84,
        PROPERTY_WRITTEN = 0x85,
        PROPERTY_SUBSCRIBED = 0x86,
        PROPERTY_UNSUBSCRIBED = 0x87,
        DATALOG_READ = 0x88,
        MESSAGES_READ = 0x89,
        PROPERTIES_FOUND = 0x8A,
        PROPERTY_UPDATE = 0xFE,
        DEVICE_MESSAGE = 0xFD
    };

    SIBluetoothProtocolFrame(Command command = INVALID, const QVector<QVariant>& parameters = {}); // NOLINT(google-explicit-constructor)

    bool isNull() const;

    inline Command command() const {
        return command_;
    }

    inline void setCommand(Command command) {
        command_ = command;
    }

    inline const QVector<QVariant>& parameters() const {
        return parameters_;
    }

    inline void setParameters(const QVector<QVariant>& parameters) {
        parameters_ = parameters;
    }

    inline int parameterCount() const {
        return parameters_.count();
    }

    bool validateParameters(const QVector<QSet<QVariant::Type>>& parameterTypes);

    QByteArray toBytes(qsizetype bufferSize = 8*1024) const;
    static SIBluetoothProtocolFrame fromBytes(const QByteArray& bytes);

    inline static SIBluetoothProtocolFrame error(const QString& reason) {
        return {ERROR, {reason}};
    }

  private:
    int encodeVariant_(CborEncoder* encoder, const QVariant& variant) const;

    Command command_;
    QVector<QVariant> parameters_;
};
