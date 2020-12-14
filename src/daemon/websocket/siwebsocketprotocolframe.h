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
        PROPERTY_UPDATE
    };

    explicit SIWebSocketProtocolFrame(Command command, std::initializer_list<QPair<QString,QString>> headers = {}, const QByteArray& body = {});

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

    inline const QByteArray& body() const {
        return body_;
    }

    inline void setBody(const QByteArray& body) {
        body_ = body;
    }

    QString toMessage();
    static SIWebSocketProtocolFrame fromMessage(QString message);

  private:
    SIWebSocketProtocolFrame() = default;

    Command command_ = INVALID;
    QMap<QString,QString> headers_;
    QByteArray body_;
};

QString to_string(SIWebSocketProtocolFrame::Command command);