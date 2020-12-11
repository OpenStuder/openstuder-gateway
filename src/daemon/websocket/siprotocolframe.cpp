#include "siprotocolframe.h"
#include <QTextStream>

SIProtocolFrame::SIProtocolFrame(SIProtocolFrame::Command command, std::initializer_list<QPair<QString, QString>> headers, const QByteArray& body)
    : command_(command), body_(body) {
    for (const auto& header: headers) {
        headers_[header.first] = header.second;
    }
}

bool SIProtocolFrame::isNull() const {
    return command_ == INVALID;
}

QString SIProtocolFrame::toMessage() {
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

SIProtocolFrame SIProtocolFrame::fromMessage(QString message) {
    SIProtocolFrame frame;
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
    } else if (commandStr == "READPROPERTY") {
        frame.command_ = READ_PROPERTY;
    } else if (commandStr == "WRITEPROPERTY") {
        frame.command_ = WRITE_PROPERTY;
    } else if (commandStr == "SUBSCRIBEPROPERTY") {
        frame.command_ = SUBSCRIBE_PROPERTY;
    } else if (commandStr == "ERROR") {
        frame.command_ = ERROR;
    } else if (commandStr == "AUTHORIZED") {
        frame.command_ = AUTHORIZED;
    } else if (commandStr == "ENUMERATED") {
        frame.command_ = ENUMERATED;
    } else if (commandStr == "DESCRIPTION") {
        frame.command_ = DESCRIPTION;
    } else if (commandStr == "PROPERTYREAD") {
        frame.command_ = PROPERTY_READ;
    } else if (commandStr == "PROPERTYWRITTEN") {
        frame.command_ = PROPERTY_WRITTEN;
    } else if (commandStr == "PROPERTYSUBSCRIBED") {
        frame.command_ = PROPERTY_SUBSCRIBED;
    } else if (commandStr == "PROPERTYUPDATE") {
        frame.command_ = PROPERTY_UPDATE;
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

QString to_string(SIProtocolFrame::Command command) {
    switch (command) {
        case SIProtocolFrame::INVALID:
            return "INVALID";

        case SIProtocolFrame::AUTHORIZE:
            return "AUTHORIZE";

        case SIProtocolFrame::ENUMERATE:
            return "ENUMERATE";

        case SIProtocolFrame::DESCRIBE:
            return "DESCRIBE";

        case SIProtocolFrame::READ_PROPERTY:
            return "READPROPERTY";

        case SIProtocolFrame::WRITE_PROPERTY:
            return "WRITEPROPERTY";

        case SIProtocolFrame::SUBSCRIBE_PROPERTY:
            return "SUBSCRIBEPROPERTY";

        case SIProtocolFrame::ERROR:
            return "ERROR";

        case SIProtocolFrame::AUTHORIZED:
            return "AUTHORIZED";

        case SIProtocolFrame::ENUMERATED:
            return "ENUMERATED";

        case SIProtocolFrame::DESCRIPTION:
            return "DESCRIPTION";

        case SIProtocolFrame::PROPERTY_READ:
            return "PROPERTYREAD";

        case SIProtocolFrame::PROPERTY_WRITTEN:
            return "PROPERTYWRITTEN";

        case SIProtocolFrame::PROPERTY_SUBSCRIBED:
            return "PROPERTYSUBSCRIBED";

        case SIProtocolFrame::PROPERTY_UPDATE:
            return "PROPERTYUPDATE";
    }
}
