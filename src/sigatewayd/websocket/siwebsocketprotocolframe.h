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
        READ_PROPERTIES,
        WRITE_PROPERTY,
        SUBSCRIBE_PROPERTY,
        SUBSCRIBE_PROPERTIES,
        UNSUBSCRIBE_PROPERTY,
        UNSUBSCRIBE_PROPERTIES,
        READ_MESSAGES,
        READ_DATALOG,
        FIND_PROPERTIES,
        CALL_EXTENSION,

        // Server messages.
        ERROR,
        AUTHORIZED,
        ENUMERATED,
        DESCRIPTION,
        PROPERTY_READ,
        PROPERTIES_READ,
        PROPERTY_WRITTEN,
        PROPERTY_SUBSCRIBED,
        PROPERTIES_SUBSCRIBED,
        PROPERTY_UNSUBSCRIBED,
        PROPERTIES_UNSUBSCRIBED,
        PROPERTY_UPDATE,
        DEVICE_MESSAGE,
        MESSAGES_READ,
        DATALOG_READ,
        PROPERTIES_FOUND,
        EXTENSION_CALLED
    };

    SIWebSocketProtocolFrame(Command command = INVALID, std::initializer_list<QPair<QString,QString>> headers = {}, QByteArray body = {}); // NOLINT(google-explicit-constructor)

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

    inline void mergeHeaders(const QMap<QString, QString>& headers) {
        for (auto i = headers.constBegin(); i != headers.constEnd(); ++i) {
            headers_.insert(i.key(), i.value());
        }
    }

    inline QString header(const QString& key, const QString& defaultValue = {}) const {
        return headers_.value(key, defaultValue);
    }

    bool validateHeaders(const std::initializer_list<const char*>& required, const std::initializer_list<const char*>& optional = {}) const;

    inline bool hasHeader(const QString& key) {
        return headers_.contains(key);
    }

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

    inline static SIWebSocketProtocolFrame error(const QString& reason) {
        return {ERROR, {{"reason", reason}}};
    }

  private:
    Command command_ = INVALID;
    QMap<QString,QString> headers_;
    QByteArray body_;
};

QString to_string(SIWebSocketProtocolFrame::Command command);