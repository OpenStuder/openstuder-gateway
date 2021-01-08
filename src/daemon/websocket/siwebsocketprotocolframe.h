#pragma once
#include <initializer_list>
#include <QPair>
#include <QString>
#include <QByteArray>
#include <QMap>

class SIWebSocketProtocolFrame {
  public:
    enum Command {
        INVALID,

        // Client messages.
        AUTHORIZE,
        ENUMERATE,
        DESCRIBE,
        READ_PROPERTY,
        WRITE_PROPERTY,
        SUBSCRIBE_PROPERTY,

        // Server messages.
        ERROR,
        AUTHORIZED,
        ENUMERATED,
        DESCRIPTION,
        PROPERTY_READ,
        PROPERTY_WRITTEN,
        PROPERTY_SUBSCRIBED,
        PROPERTY_UPDATE,
        DEVICE_MESSAGE
    };

    SIWebSocketProtocolFrame(Command command = INVALID, std::initializer_list<QPair<QString,QString>> headers = {}, const QByteArray& body = {});

    bool isNull() const;

    inline Command command() const {
        return command_;
    }

    inline void setCommand(Command command) {
        command_ = command;
    }

    inline const QMap<QString, QString>& headers() const {
        return headers_;
    }

    inline void setHeaders(const QMap<QString, QString>& headers) {
        headers_ = headers;
    }

    inline QString header(const QString& key, const QString& defaultValue = {}) const {
        return headers_.value(key, defaultValue);
    }

    bool validateHeaders(const std::initializer_list<const char*>& required, const std::initializer_list<const char*>& optional = {}) const;

    inline const QByteArray& body() const {
        return body_;
    }

    inline void setBody(const QByteArray& body) {
        body_ = body;
    }

    inline bool hasBody() const {
        return !body_.isEmpty();
    }

    QString toMessage() const;
    static SIWebSocketProtocolFrame fromMessage(QString message);

  private:
    Command command_ = INVALID;
    QMap<QString,QString> headers_;
    QByteArray body_;
};

QString to_string(SIWebSocketProtocolFrame::Command command);