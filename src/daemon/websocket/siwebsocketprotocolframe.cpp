#include "siwebsocketprotocolframe.h"
#include <QTextStream>

SIWebSocketProtocolFrame::SIWebSocketProtocolFrame(SIWebSocketProtocolFrame::Command command, std::initializer_list<QPair<QString, QString>> headers, const QByteArray& body)
    : command_(command), body_(body) {
    for (const auto& header: headers) {
        headers_[header.first] = header.second;
    }
}

bool SIWebSocketProtocolFrame::isNull() const {
    return command_ == INVALID;
}

QString SIWebSocketProtocolFrame::toMessage() {
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
    } else if (commandStr == "WRITE PROPERTY") {
        frame.command_ = WRITE_PROPERTY;
    } else if (commandStr == "SUBSCRIBE PROPERTY") {
        frame.command_ = SUBSCRIBE_PROPERTY;
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
    } else if (commandStr == "PROPERTY WRITTEN") {
        frame.command_ = PROPERTY_WRITTEN;
    } else if (commandStr == "PROPERTY SUBSCRIBED") {
        frame.command_ = PROPERTY_SUBSCRIBED;
    } else if (commandStr == "PROPERTY UPDATE") {
        frame.command_ = PROPERTY_UPDATE;
    } else if (commandStr == "MESSAGE") {
        frame.command_ = MESSAGE;
    }

    QString headerLine;
    while (in.readLineInto(&headerLine) && headerLine != "") {
        auto components = headerLine.split(':');
        if (components.count() != 2) {
            return {};
        }
        frame.headers_.insert(components[0].trimmed(), components[1].trimmed());
    }

    in >> frame.body_;

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

        case SIWebSocketProtocolFrame::WRITE_PROPERTY:
            return "WRITE PROPERTY";

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTY:
            return "SUBSCRIBE PROPERTY";

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

        case SIWebSocketProtocolFrame::PROPERTY_WRITTEN:
            return "PROPERTY WRITTEN";

        case SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED:
            return "PROPERTY SUBSCRIBED";

        case SIWebSocketProtocolFrame::PROPERTY_UPDATE:
            return "PROPERTY UPDATE";

        case SIWebSocketProtocolFrame::MESSAGE:
            return "MESSAGE";

        default:
            return "INVALID";
    }
}
