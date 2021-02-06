#include "siwebsocketprotocolv1.h"
#include "sistorage.h"
#include <sijsonflags.h>
#include <sideviceaccessregistry.h>
#include <sideviceaccess.h>
#include <sidevice.h>
#include <QJsonDocument>
#include <QJsonArray>

SIWebSocketProtocolV1::SIWebSocketProtocolV1(SIAccessLevel accessLevel): accessLevel_(accessLevel) {}

SIWebSocketProtocolFrame SIWebSocketProtocolV1::handleFrame(SIWebSocketProtocolFrame& frame, SIContext& context) {
    // Frames send from the client can never have a body!
    if (frame.hasBody()) {
        return SIWebSocketProtocolFrame::error("invalid frame");
    }

    switch (frame.command()) {
        case SIWebSocketProtocolFrame::AUTHORIZE:
            return SIWebSocketProtocolFrame::error("invalid state");

        case SIWebSocketProtocolFrame::ENUMERATE: {
            if (frame.headers().count() != 0) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto* operation = context.deviceAccessManager().enumerateDevices();
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::enumerationOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::DESCRIBE: {
            if (!frame.validateHeaders({}, {"id", "flags"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            SIJsonFlags jsonFlags = SIJsonFlag::Default;
            if (frame.hasHeader("flags")) {
                jsonFlags = SIJsonFlag::None;
                for (const auto& flag: frame.header("flags").split(",")) {
                    if (flag == "IncludeAccessInformation") jsonFlags |= SIJsonFlag::IncludeAccessInformation;
                    else if (flag == "IncludeAccessDetails") jsonFlags |= SIJsonFlag::IncludeAccessDetails;
                    else if (flag == "IncludeDeviceDetails") jsonFlags |= SIJsonFlag::IncludeDeviceDetails;
                    else if (flag == "IncludeDriverInformation") jsonFlags |= SIJsonFlag::IncludeDriverInformation;
                    else {
                        return SIWebSocketProtocolFrame::error("invalid frame");
                    }
                }
            }

            if (frame.hasHeader("id")) {
                auto id = frame.header("id").split(".");
                switch (id.count()) {
                    case 1: {
                        auto deviceAccess = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(id[0]);
                        if (deviceAccess == nullptr) {
                            return {SIWebSocketProtocolFrame::DESCRIPTION, {
                                {"status", to_string(SIStatus::NoDeviceAccess)},
                                {"id", frame.header("id")}
                            }};
                        }

                        return {SIWebSocketProtocolFrame::DESCRIPTION, {
                            {"status", to_string(SIStatus::Success)},
                            {"id", frame.header("id")}
                        }, QJsonDocument(deviceAccess->jsonDescription(accessLevel_, jsonFlags)).toJson(QJsonDocument::Compact)};
                    }

                    case 2: {
                        auto deviceAccess = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(id[0]);
                        if (deviceAccess == nullptr) {
                            return {SIWebSocketProtocolFrame::DESCRIPTION, {
                                {"status", to_string(SIStatus::NoDeviceAccess)},
                                {"id", frame.header("id")}
                            }};
                        }

                        auto device = deviceAccess->device(id[1]);
                        if (device == nullptr) {
                            return {SIWebSocketProtocolFrame::DESCRIPTION, {
                                {"status", to_string(SIStatus::NoDevice)},
                                {"id", frame.header("id")}
                            }};
                        }

                        return {SIWebSocketProtocolFrame::DESCRIPTION, {
                            {"status", to_string(SIStatus::Success)},
                            {"id", frame.header("id")}
                        }, QJsonDocument(device->jsonDescription(accessLevel_, jsonFlags)).toJson(QJsonDocument::Compact)};
                    }

                    default:
                        return SIWebSocketProtocolFrame::error("invalid frame");
                }
            } else {
                auto description = SIDeviceAccessRegistry::sharedRegistry().jsonDescription(accessLevel_, jsonFlags);
                return {SIWebSocketProtocolFrame::DESCRIPTION, {
                    {"status", to_string(SIStatus::Success)}
                }, QJsonDocument(description).toJson(QJsonDocument::Compact)};
            }
        }

        case SIWebSocketProtocolFrame::READ_PROPERTY: {
            if (!frame.validateHeaders({"id"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type == SIPropertyType::Invalid || accessLevel_ < property.accessLevel) {
                return {SIWebSocketProtocolFrame::PROPERTY_READ, {
                    {"status", to_string(SIStatus::NoProperty)},
                    {"id", frame.header("id")}
                }};
            }
            if (!property.flags.testFlag(SIPropertyFlag::Readable)) {
                return {SIWebSocketProtocolFrame::PROPERTY_READ, {
                    {"status", to_string(SIStatus::Error)},
                    {"id", frame.header("id")}
                }};
            }

            auto* operation = context.deviceAccessManager().readProperty(id);
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::readPropertyOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::WRITE_PROPERTY: {
            if (!frame.validateHeaders({"id"}, {"value", "flags"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type == SIPropertyType::Invalid || accessLevel_ < property.accessLevel) {
                return {SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
                    {"status", to_string(SIStatus::NoProperty)}
                }};
            }
            if (!property.flags.testFlag(SIPropertyFlag::Writeable)) {
                return {SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
                    {"status", to_string(SIStatus::Error)}
                }};
            }

            SIPropertyWriteFlags writeFlags = SIPropertyWriteFlag::Default;
            if (frame.hasHeader("flags")) {
                writeFlags = SIPropertyWriteFlag::None;
                for (const auto& flag: frame.header("flags").split(",")) {
                    if (flag == "Permanent") writeFlags |= SIPropertyWriteFlag::Permanent;
                    else {
                        return SIWebSocketProtocolFrame::error("invalid frame");
                    }
                }
            }

            auto value = frame.header("value");
            auto* operation = context.deviceAccessManager().writeProperty(id, value, writeFlags);
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::writePropertyOperationFinished_);
            break;
        }

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTY: {
            if (!frame.validateHeaders({"id"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type == SIPropertyType::Invalid || accessLevel_ < property.accessLevel) {
                return {SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED, {
                    {"status", to_string(SIStatus::NoProperty)},
                    {"id", frame.header("id")}
                }};
            }
            if (!property.flags.testFlag(SIPropertyFlag::Readable)) {
                return {SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED, {
                    {"status", to_string(SIStatus::Error)},
                    {"id", frame.header("id")}
                }};
            }

            bool status = context.deviceAccessManager().subscribeToProperty(id, this);
            return {SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED, {
                {"status", to_string(status ? SIStatus::Success : SIStatus::Error)},
                {"id", id.toString()}
            }};
        }

        case SIWebSocketProtocolFrame::UNSUBSCRIBE_PROPERTY: {
            if (!frame.validateHeaders({"id"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            if (!id.isValid()) {
                return {SIWebSocketProtocolFrame::PROPERTY_UNSUBSCRIBED, {
                    {"status", to_string(SIStatus::NoProperty)},
                    {"id", frame.header("id")}
                }};
            }

            bool status = context.deviceAccessManager().unsubscribeFromProperty(id, this);
            return {SIWebSocketProtocolFrame::PROPERTY_UNSUBSCRIBED, {
                {"status", to_string(status ? SIStatus::Success : SIStatus::Error)},
                {"id", id.toString()}
            }};
        }

        case SIWebSocketProtocolFrame::READ_MESSAGES: {
            if (!frame.validateHeaders({}, {"from", "to", "limit"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto from = QDateTime::fromMSecsSinceEpoch(0);
            if (frame.hasHeader("from")) {
                from = QDateTime::fromString(frame.header("from"), Qt::ISODate);
                if (!from.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto to = QDateTime::currentDateTime();
            if (frame.hasHeader("to")) {
                to = QDateTime::fromString(frame.header("to"), Qt::ISODate);
                if (!from.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto limit = std::numeric_limits<unsigned int>::max();
            if (frame.hasHeader("limit")) {
                bool conversionOk = false;
                limit = frame.header("limit").toUInt(&conversionOk);
                if (!conversionOk) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto messages = context.storage().retrieveDeviceMessages(from, to, limit);

            // Encode JSON messages array
            QJsonArray jsonMessages;
            for (const auto& message: messages) {
                QJsonObject jsonMessage;
                jsonMessage["timestamp"] = message.timestamp.toString(Qt::ISODate);
                jsonMessage["access_id"] = message.message.accessID;
                jsonMessage["device_id"] = message.message.deviceID;
                jsonMessage["message_id"] = (qlonglong)message.message.messageID;
                jsonMessage["message"] = message.message.message;
                jsonMessages.append(jsonMessage);
            }

            return {SIWebSocketProtocolFrame::MESSAGES_READ, {
                {"status", to_string(SIStatus::Success)},
                {"count", QString::number(messages.count())}
            }, QJsonDocument(jsonMessages).toJson(QJsonDocument::Compact)};
        }

        case SIWebSocketProtocolFrame::READ_DATALOG: {
            if (!frame.validateHeaders({"id"}, {"from", "to", "limit"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            if (!id.isValid()) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto from = QDateTime::fromMSecsSinceEpoch(0);
            if (frame.hasHeader("from")) {
                from = QDateTime::fromString(frame.header("from"), Qt::ISODate);
                if (!from.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto to = QDateTime::currentDateTime();
            if (frame.hasHeader("to")) {
                to = QDateTime::fromString(frame.header("to"), Qt::ISODate);
                if (!from.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto limit = std::numeric_limits<unsigned int>::max();
            if (frame.hasHeader("limit")) {
                bool conversionOk = false;
                limit = frame.header("limit").toUInt(&conversionOk);
                if (!conversionOk) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto data = context.storage().retrievePropertyValues(id, from, to, limit);

            QString buffer;
            QTextStream output(&buffer);
            for (const auto& entry: data) {
                output << entry.timestamp.toString(Qt::ISODate) << ", " << entry.value.toString() << "\n";
            }
            output.flush();

            return {SIWebSocketProtocolFrame::DATALOG_READ, {
                {"count", QString::number(data.count())}
            }, buffer.toUtf8()};
        }

        default:
            return SIWebSocketProtocolFrame::error("invalid frame");
    }

    return {};
}

SIWebSocketProtocolFrame SIWebSocketProtocolV1::convertDeviceMessage(const SIDeviceMessage& message) {
    return {SIWebSocketProtocolFrame::DEVICE_MESSAGE, {
        {"access_id", message.accessID},
        {"device_id", message.deviceID},
        {"message_id", QString::number(message.messageID)},
        {"message", message.message}
    }};
}

void SIWebSocketProtocolV1::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_UPDATE, {
        {"id", id.toString()},
        {"value", value.toString()}
    }});
}

void SIWebSocketProtocolV1::enumerationOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIDeviceEnumerationOperation*>(sender());
    emit frameReadyToSend({SIWebSocketProtocolFrame::ENUMERATED, {
        {"status", to_string(status)},
        {"device_count", QString::number(operation->numberOfDevicesPresent())}
    }});
    delete operation;
}

void SIWebSocketProtocolV1::readPropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyReadOperation*>(sender());
    if (operation->status() == SIStatus::Success) {
        emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_READ, {
            {"status", to_string(status)},
            {"id", operation->id().toString()},
            {"value", operation->value().toString()}
        }});
    } else {
        emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_READ, {
            {"status", to_string(status)},
            {"id", operation->id().toString()}
        }});
    }
    delete operation;
}

void SIWebSocketProtocolV1::writePropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyWriteOperation*>(sender());
    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
        {"status", to_string(status)},
        {"id", operation->id().toString()}
    }});
    delete operation;
}
