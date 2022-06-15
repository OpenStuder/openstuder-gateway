#include "siwebsocketprotocolframe.h"
#include <QTextStream>
#include <utility>

SIWebSocketProtocolFrame::SIWebSocketProtocolFrame(SIWebSocketProtocolFrame::Command command, std::initializer_list<QPair<QString, QString>> headers, QByteArray body)
    : command_(command), body_(std::move(body)) {
    for (const auto& header: headers) {
        headers_[header.first] = header.second;
    }
}

bool SIWebSocketProtocolFrame::isNull() const {
    return command_ == INVALID;
}

bool SIWebSocketProtocolFrame::validateHeaders(const std::initializer_list<const char*>& required, const std::initializer_list<const char*>& optional) const {
    auto keys = headers_.keys();

    // Check that all required headers are present.
    for (const auto& key: required) {
        if (!keys.contains(key)) return false;
        keys.removeOne(key);
    }

    // Check that no other than option headers are present.
    for (const auto& key: optional) {
        keys.removeOne(key);
    }
    return keys.isEmpty();
}

QString SIWebSocketProtocolFrame::toMessage() const {
    QString encoded;
    QTextStream out(&encoded, QIODevice::WriteOnly);

    out << to_string(command_) << "\n";

    for (const auto& key: headers_.keys()) {
        out << key << ":" << headers_[key] << "\n";
    }

    out << "\n" << body_;

    out.flush();

    return encoded;
}

SIWebSocketProtocolFrame SIWebSocketProtocolFrame::fromMessage(QString message) {
    SIWebSocketProtocolFrame frame;
    QTextStream in(&message, QIODevice::ReadOnly);

    QString commandStr;
    if (!in.readLineInto(&commandStr)) {
        return frame;
    }

    if (commandStr == "AUTHORIZE") {
        frame.command_ = AUTHORIZE;
    } else if (commandStr == "ENUMERATE") {
        frame.command_ = ENUMERATE;
    } else if (commandStr == "DESCRIBE") {
        frame.command_ = DESCRIBE;
    } else if (commandStr == "READ PROPERTY") {
        frame.command_ = READ_PROPERTY;
    } else if (commandStr == "READ PROPERTIES") {
        frame.command_ = READ_PROPERTIES;
    } else if (commandStr == "WRITE PROPERTY") {
        frame.command_ = WRITE_PROPERTY;
    } else if (commandStr == "SUBSCRIBE PROPERTY") {
        frame.command_ = SUBSCRIBE_PROPERTY;
    } else if (commandStr == "SUBSCRIBE PROPERTIES") {
        frame.command_ = SUBSCRIBE_PROPERTIES;
    } else if (commandStr == "UNSUBSCRIBE PROPERTY") {
        frame.command_ = UNSUBSCRIBE_PROPERTY;
    } else if (commandStr == "UNSUBSCRIBE PROPERTIES") {
        frame.command_ = UNSUBSCRIBE_PROPERTIES;
    } else if (commandStr == "READ MESSAGES") {
        frame.command_ = READ_MESSAGES;
    } else if (commandStr == "READ DATALOG") {
        frame.command_ = READ_DATALOG;
    } else if (commandStr == "LIST EXTENSIONS") {
        frame.command_ = LIST_EXTENSIONS;
    } else if (commandStr == "CALL EXTENSION") {
        frame.command_ = CALL_EXTENSION;
    } else if (commandStr == "ERROR") {
        frame.command_ = ERROR;
    } else if (commandStr == "AUTHORIZED") {
        frame.command_ = AUTHORIZED;
    } else if (commandStr == "ENUMERATED") {
        frame.command_ = ENUMERATED;
    } else if (commandStr == "DESCRIPTION") {
        frame.command_ = DESCRIPTION;
    } else if (commandStr == "PROPERTY READ") {
        frame.command_ = PROPERTY_READ;
    } else if (commandStr == "PROPERTIES READ") {
        frame.command_ = PROPERTIES_READ;
    } else if (commandStr == "PROPERTY WRITTEN") {
        frame.command_ = PROPERTY_WRITTEN;
    } else if (commandStr == "PROPERTY SUBSCRIBED") {
        frame.command_ = PROPERTY_SUBSCRIBED;
    } else if (commandStr == "PROPERTIES SUBSCRIBED") {
        frame.command_ = PROPERTIES_SUBSCRIBED;
    } else if (commandStr == "PROPERTY UNSUBSCRIBED") {
        frame.command_ = PROPERTY_UNSUBSCRIBED;
    } else if (commandStr == "PROPERTIES UNSUBSCRIBED") {
        frame.command_ = PROPERTIES_UNSUBSCRIBED;
    } else if (commandStr == "PROPERTY UPDATE") {
        frame.command_ = PROPERTY_UPDATE;
    } else if (commandStr == "DEVICE MESSAGE") {
        frame.command_ = DEVICE_MESSAGE;
    } else if (commandStr == "MESSAGES_READ") {
        frame.command_ = MESSAGES_READ;
    } else if (commandStr == "DATALOG READ") {
        frame.command_ = DATALOG_READ;
    } else if (commandStr == "FIND PROPERTIES") {
        frame.command_ = FIND_PROPERTIES;
    } else if (commandStr == "PROPERTIES FOUND") {
        frame.command_ = PROPERTIES_FOUND;
    } else if (commandStr == "EXTENSIONS LIST") {
        frame.command_ = EXTENSIONS_LIST;
    } else if (commandStr == "EXTENSION CALLED") {
        frame.command_ = EXTENSION_CALLED;
    }

    QString headerLine;
    while (in.readLineInto(&headerLine) && headerLine != "") {
        auto components = headerLine.split(':');
        if (components.count() < 2) {
            return {};
        }
        frame.headers_.insert(components[0].trimmed(), components.mid(1).join(':').trimmed());
    }

    frame.body_ = in.readAll().toUtf8();

    return frame;
}

QString to_string(SIWebSocketProtocolFrame::Command command) {
    switch (command) {
        case SIWebSocketProtocolFrame::AUTHORIZE:
            return "AUTHORIZE";

        case SIWebSocketProtocolFrame::ENUMERATE:
            return "ENUMERATE";

        case SIWebSocketProtocolFrame::DESCRIBE:
            return "DESCRIBE";

        case SIWebSocketProtocolFrame::READ_PROPERTY:
            return "READ PROPERTY";

        case SIWebSocketProtocolFrame::READ_PROPERTIES:
            return "READ PROPERTIES";

        case SIWebSocketProtocolFrame::WRITE_PROPERTY:
            return "WRITE PROPERTY";

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTY:
            return "SUBSCRIBE PROPERTY";

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTIES:
            return "SUBSCRIBE PROPERTIES";

        case SIWebSocketProtocolFrame::UNSUBSCRIBE_PROPERTY:
            return "UNSUBSCRIBE PROPERTY";

        case SIWebSocketProtocolFrame::UNSUBSCRIBE_PROPERTIES:
            return "UNSUBSCRIBE PROPERTIES";

        case SIWebSocketProtocolFrame::READ_MESSAGES:
            return "READ MESSAGES";

        case SIWebSocketProtocolFrame::READ_DATALOG:
            return "READ DATALOG";

        case SIWebSocketProtocolFrame::LIST_EXTENSIONS:
            return "LIST EXTENSIONS";

        case SIWebSocketProtocolFrame::CALL_EXTENSION:
            return "CALL EXTENSION";

        case SIWebSocketProtocolFrame::ERROR:
            return "ERROR";

        case SIWebSocketProtocolFrame::AUTHORIZED:
            return "AUTHORIZED";

        case SIWebSocketProtocolFrame::ENUMERATED:
            return "ENUMERATED";

        case SIWebSocketProtocolFrame::DESCRIPTION:
            return "DESCRIPTION";

        case SIWebSocketProtocolFrame::PROPERTY_READ:
            return "PROPERTY READ";

        case SIWebSocketProtocolFrame::PROPERTIES_READ:
            return "PROPERTIES READ";

        case SIWebSocketProtocolFrame::PROPERTY_WRITTEN:
            return "PROPERTY WRITTEN";

        case SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED:
            return "PROPERTY SUBSCRIBED";

        case SIWebSocketProtocolFrame::PROPERTIES_SUBSCRIBED:
            return "PROPERTIES SUBSCRIBED";

        case SIWebSocketProtocolFrame::PROPERTY_UNSUBSCRIBED:
            return "PROPERTY UNSUBSCRIBED";

        case SIWebSocketProtocolFrame::PROPERTIES_UNSUBSCRIBED:
            return "PROPERTIES UNSUBSCRIBED";

        case SIWebSocketProtocolFrame::PROPERTY_UPDATE:
            return "PROPERTY UPDATE";

        case SIWebSocketProtocolFrame::DEVICE_MESSAGE:
            return "DEVICE MESSAGE";

        case SIWebSocketProtocolFrame::MESSAGES_READ:
            return "MESSAGES READ";

        case SIWebSocketProtocolFrame::DATALOG_READ:
            return "DATALOG READ";

        case SIWebSocketProtocolFrame::FIND_PROPERTIES:
            return "FIND PROPERTIES";

        case SIWebSocketProtocolFrame::PROPERTIES_FOUND:
            return "PROPERTIES FOUND";

        case SIWebSocketProtocolFrame::EXTENSIONS_LIST:
            return "EXTENSION LIST";

        case SIWebSocketProtocolFrame::EXTENSION_CALLED:
            return "EXTENSION CALLED";

        default:
            return "INVALID";
    }
}
