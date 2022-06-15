#include "siextension.h"

struct SIExtension::Private_ {
    QString id;
};

SIExtension::SIExtension(const QString& id): private_(new Private_) {
    private_->id = id;
}

SIExtension::~SIExtension() = default;

const QString& SIExtension::id() const {
    return private_->id;
}

const QStringList& SIExtension::commands() const {
    return commands_();
}

SIExtension::Result SIExtension::runCommand(const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) {
    return runCommand_(command, headers, body);
}

QString to_string(SIExtension::Status status) {
    switch (status) {
        case SIExtension::Status::Success:
            return "Success";

        case SIExtension::Status::UnsupportedExtension:
            return "UnsupportedExtension";

        case SIExtension::Status::UnsupportedCommand:
            return "UnsupportedCommand";

        case SIExtension::Status::InvalidHeaders:
            return "InvalidHeaders";

        case SIExtension::Status::InvalidBody:
            return "InvalidBody";

        case SIExtension::Status::Error:
            return "Error";

        default:
            return "Invalid";
    }
}
