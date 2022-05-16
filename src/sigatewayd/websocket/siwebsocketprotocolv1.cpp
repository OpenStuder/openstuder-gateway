#include "siwebsocketprotocolv1.h"
#include "sistorage.h"
#include <sijdescriptionflags.h>
#include <sideviceaccessregistry.h>
#include <sideviceaccess.h>
#include <sidevice.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>

SIWebSocketProtocolV1::SIWebSocketProtocolV1(SIAccessLevel accessLevel): accessLevel_(accessLevel) {}

SIWebSocketProtocolFrame SIWebSocketProtocolV1::handleFrame(SIWebSocketProtocolFrame& frame, SIContext& context) {
    switch (frame.command()) {
        case SIWebSocketProtocolFrame::AUTHORIZE:
            return SIWebSocketProtocolFrame::error("invalid state");

        case SIWebSocketProtocolFrame::ENUMERATE: {
            if (frame.hasBody() || frame.headers().count() != 0) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto* operation = context.deviceAccessManager().enumerateDevices();
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::enumerationOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::DESCRIBE: {
            if (frame.hasBody() || !frame.validateHeaders({}, {"id", "flags"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            SIDescriptionFlags jsonFlags = SIDescriptionFlag::Default;
            if (frame.hasHeader("flags")) {
                jsonFlags = SIDescriptionFlag::None;
                for (const auto& flag: frame.header("flags").split(",")) {
                    if (flag == "IncludeAccessInformation") { jsonFlags |= SIDescriptionFlag::IncludeAccessInformation; }
                    else if (flag == "IncludeDeviceInformation") { jsonFlags |= SIDescriptionFlag::IncludeDeviceInformation; }
                    else if (flag == "IncludePropertyInformation") { jsonFlags |= SIDescriptionFlag::IncludePropertyInformation; }
                    else if (flag == "IncludeDriverInformation") { jsonFlags |= SIDescriptionFlag::IncludeDriverInformation; }
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

                    case 3: {
                        auto property = context.deviceAccessManager().resolveProperty(SIGlobalPropertyID(frame.header("id")));
                        if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                            return {SIWebSocketProtocolFrame::DESCRIPTION, {
                                {"status", to_string(SIStatus::NoProperty)},
                                {"id", frame.header("id")}
                            }};
                        }

                        return {SIWebSocketProtocolFrame::DESCRIPTION, {
                            {"status", to_string(SIStatus::Success)},
                            {"id", frame.header("id")}
                        }, QJsonDocument(property.jsonDescription(jsonFlags)).toJson(QJsonDocument::Compact)};
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
            if (frame.hasBody() || !frame.validateHeaders({"id"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIWebSocketProtocolFrame::PROPERTY_READ, {
                    {"status", to_string(SIStatus::NoProperty)},
                    {"id", frame.header("id")}
                }};
            }
            if (!property.isFlagSet(SIPropertyFlag::Readable)) {
                return {SIWebSocketProtocolFrame::PROPERTY_READ, {
                    {"status", to_string(SIStatus::Error)},
                    {"id", frame.header("id")}
                }};
            }

            auto* operation = context.deviceAccessManager().readProperty(id);
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::readPropertyOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::READ_PROPERTIES: {
            if (!frame.hasBody() || !frame.validateHeaders({})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto idsDocument = QJsonDocument::fromJson(frame.body());
            if (!idsDocument.isArray() || idsDocument.array().empty()) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            QVector<SIGlobalPropertyID> ids;
            for (auto entry: idsDocument.array()) {
                SIGlobalPropertyID id {entry.toString()};
                if (!id.isValid() || id.isWildcard()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
                ids << id;
            }

            auto* operation = context.deviceAccessManager().readProperties(ids);
            connect(operation, &SIAbstractOperation::finished, this, &SIWebSocketProtocolV1::readPropertiesOperationFinished_);
            return {};
        }

        case SIWebSocketProtocolFrame::WRITE_PROPERTY: {
            if (frame.hasBody() || !frame.validateHeaders({"id"}, {"value", "flags"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
                    {"status", to_string(SIStatus::NoProperty)},
                    {"id", frame.header("id")}
                }};
            }
            if (!property.isFlagSet(SIPropertyFlag::Writeable)) {
                return {SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
                    {"status", to_string(SIStatus::Error)},
                    {"id", frame.header("id")}
                }};
            }

            SIPropertyWriteFlags writeFlags = SIPropertyWriteFlag::Default;
            if (frame.hasHeader("flags")) {
                writeFlags = SIPropertyWriteFlag::None;
                for (const auto& flag: frame.header("flags").split(",")) {
                    if (flag == "Permanent") { writeFlags |= SIPropertyWriteFlag::Permanent; }
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
            if (frame.hasBody() || !frame.validateHeaders({"id"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID(frame.header("id"));
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIWebSocketProtocolFrame::PROPERTY_SUBSCRIBED, {
                    {"status", to_string(SIStatus::NoProperty)},
                    {"id", frame.header("id")}
                }};
            }
            if (!property.isFlagSet(SIPropertyFlag::Readable)) {
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

        case SIWebSocketProtocolFrame::SUBSCRIBE_PROPERTIES: {
            if (!frame.hasBody() || !frame.validateHeaders({})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto requestJSON = QJsonDocument::fromJson(frame.body());
            if (!requestJSON.isArray()) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            QJsonArray responseJSON;
            for (auto entry: requestJSON.array()) {
                SIGlobalPropertyID id {entry.toString()};

                auto property = context.deviceAccessManager().resolveProperty(id);

                if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                    responseJSON.append(QJsonObject {
                        {"status", to_string(SIStatus::NoProperty)},
                        {"id",     id.toString()}
                    });
                    continue;
                }

                if (!property.isFlagSet(SIPropertyFlag::Readable)) {
                    responseJSON.append(QJsonObject {
                        {"status", to_string(SIStatus::Error)},
                        {"id",     id.toString()}
                    });
                    continue;
                }

                bool status = context.deviceAccessManager().subscribeToProperty(id, this);
                responseJSON.append(QJsonObject {
                    {"status", to_string(status ? SIStatus::Success : SIStatus::Error)},
                    {"id",     id.toString()}
                });
            }

            return {SIWebSocketProtocolFrame::PROPERTIES_SUBSCRIBED, {
                {"status", to_string(SIStatus::Success)},
            }, QJsonDocument(responseJSON).toJson(QJsonDocument::Compact)};
        }

        case SIWebSocketProtocolFrame::UNSUBSCRIBE_PROPERTY: {
            if (frame.hasBody() || !frame.validateHeaders({"id"})) {
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

        case SIWebSocketProtocolFrame::UNSUBSCRIBE_PROPERTIES: {
            if (!frame.hasBody() || !frame.validateHeaders({})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto requestJSON = QJsonDocument::fromJson(frame.body());
            if (!requestJSON.isArray()) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            QJsonArray responseJSON;
            for (auto entry: requestJSON.array()) {
                SIGlobalPropertyID id {entry.toString()};
                if (!id.isValid()) {
                    responseJSON.append(QJsonObject {
                        {"status", to_string(SIStatus::NoProperty)},
                        {"id",     id.toString()}
                    });
                    continue;
                }

                bool status = context.deviceAccessManager().unsubscribeFromProperty(id, this);
                responseJSON.append(QJsonObject {
                    {"status", to_string(status ? SIStatus::Success : SIStatus::Error)},
                    {"id",     id.toString()}
                });
            }

            return {SIWebSocketProtocolFrame::PROPERTIES_UNSUBSCRIBED, {
                {"status", to_string(SIStatus::Success)},
            }, QJsonDocument(responseJSON).toJson(QJsonDocument::Compact)};
        }

        case SIWebSocketProtocolFrame::READ_MESSAGES: {
            if (frame.hasBody() || !frame.validateHeaders({}, {"from", "to", "limit"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto from = QDateTime::fromMSecsSinceEpoch(0);
            if (frame.hasHeader("from")) {
                from = QDateTime::fromString(frame.header("from"), Qt::ISODate);
                if (!from.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto to = QDateTime::currentDateTimeUtc();
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

            SIStatus status;
            auto messages = context.storage().retrieveDeviceMessages(from, to, limit, &status);

            // Encode JSON messages array
            QJsonArray jsonMessages;
            for (auto message = messages.crbegin(); message != messages.crend(); ++message) {
                QJsonObject jsonMessage;
                jsonMessage["timestamp"] = message->timestamp().toUTC().toString(Qt::ISODate);
                jsonMessage["access_id"] = message->accessID();
                jsonMessage["device_id"] = message->deviceID();
                jsonMessage["message_id"] = (qlonglong)message->messageID();
                jsonMessage["message"] = message->message();
                jsonMessages.append(jsonMessage);
            }

            return {SIWebSocketProtocolFrame::MESSAGES_READ, {
                {"status", to_string(status)},
                {"count", QString::number(messages.count())}
            }, QJsonDocument(jsonMessages).toJson(QJsonDocument::Compact)};
        }

        case SIWebSocketProtocolFrame::READ_DATALOG: {
            if (frame.hasBody() || !frame.validateHeaders({}, {"id", "from", "to", "limit"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            auto id = SIGlobalPropertyID();
            if (frame.hasHeader("id")) {
                id = SIGlobalPropertyID(frame.header("id"));
                if (!id.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto from = QDateTime::fromMSecsSinceEpoch(0);
            if (frame.hasHeader("from")) {
                from = QDateTime::fromString(frame.header("from"), Qt::ISODate);
                if (!from.isValid()) {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto to = QDateTime::currentDateTimeUtc();
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

            if (id.isValid()) {
                SIStatus status;
                auto data = context.storage().retrievePropertyValues(id, from, to, limit, &status);

                QString buffer;
                QTextStream output(&buffer);
                for (auto entry = data.crbegin(); entry != data.crend(); ++entry) {
                    output << entry->timestamp.toUTC().toString(Qt::ISODate) << "," << entry->value.toString() << "\n";
                }
                output.flush();

                return {SIWebSocketProtocolFrame::DATALOG_READ, {
                    {"status", to_string(status)},
                    {"id", id.toString()},
                    {"count", QString::number(data.count())}
                }, buffer.toUtf8()};
            } else {
                SIStatus status;
                auto storedPropertyIDs = context.storage().availableStoredProperties(from, to, &status);

                QString buffer;
                QTextStream output(&buffer);
                for (const auto & propertyID : storedPropertyIDs) {
                    output << propertyID.toString() << "\n";
                }
                output.flush();

                return {SIWebSocketProtocolFrame::DATALOG_READ, {
                    {"status", to_string(status)},
                    {"count", QString::number(storedPropertyIDs.count())}
                }, buffer.toUtf8()};
            }
        }

        case SIWebSocketProtocolFrame::FIND_PROPERTIES: {
            if (frame.hasBody() || !frame.validateHeaders({"id"}, {"virtual", "functions"})) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            SIGlobalPropertyID id = frame.header("id");
            if (!id.isValid()) {
                return SIWebSocketProtocolFrame::error("invalid frame");
            }

            bool virtualDevices = frame.header("virtual", "false").toLower() == "true";

            SIDeviceFunctions functionsMask = frame.hasHeader("functions") ? SIDeviceFunction::None : SIDeviceFunction::All;
            for (const auto& functionString: frame.header("functions", "").split(",")) {
                if (functionString.toLower() == "inverter") functionsMask |= SIDeviceFunction::Inverter;
                else if (functionString.toLower() == "charger") functionsMask |= SIDeviceFunction::Charger;
                else if (functionString.toLower() == "solar") functionsMask |= SIDeviceFunction::Solar;
                else if (functionString.toLower() == "transfer") functionsMask |= SIDeviceFunction::Transfer;
                else if (functionString.toLower() == "battery") functionsMask |= SIDeviceFunction::Battery;
                else if (functionString.toLower() == "all") functionsMask |= SIDeviceFunction::All;
                else {
                    return SIWebSocketProtocolFrame::error("invalid frame");
                }
            }

            auto propertyIDs = context.deviceAccessManager().findProperties(id, virtualDevices, functionsMask);
            QJsonArray propertyIDsJSON;
            for (const auto& id: propertyIDs) propertyIDsJSON << id.toString();

            return {SIWebSocketProtocolFrame::PROPERTIES_FOUND, {
                {"status", to_string(SIStatus::Success)},
                {"id", frame.header("id")},
                {"count", QString::number(propertyIDs.count())}
            }, QJsonDocument(propertyIDsJSON).toJson(QJsonDocument::Compact)};
        }

        default:
            return SIWebSocketProtocolFrame::error("invalid frame");
    }

    return {};
}

SIWebSocketProtocolFrame SIWebSocketProtocolV1::convertDeviceMessage(const SIDeviceMessage& message) {
    return {SIWebSocketProtocolFrame::DEVICE_MESSAGE, {
        {"timestamp", message.timestamp().toUTC().toString(Qt::ISODate)},
        {"access_id", message.accessID()},
        {"device_id", message.deviceID()},
        {"message_id", QString::number(message.messageID())},
        {"message", message.message()}
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
    operation->deleteLater();
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
    operation->deleteLater();
}

void SIWebSocketProtocolV1::readPropertiesOperationFinished_(SIStatus status) {
    Q_UNUSED(status)

    auto* operations = dynamic_cast<SIPropertiesReadOperation*>(sender());

    QJsonArray json;
    for (int i = 0; i < operations->count(); ++i) {
        const auto& operation = (*operations)[i];

        json.append(QJsonObject {
            {"status", to_string(operation.status())},
            {"id",     operation.id().toString()},
            {"value",  QJsonValue::fromVariant(operation.value())}
        });
    }

    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTIES_READ, {
        {"status", to_string(SIStatus::Success)},
    }, QJsonDocument(json).toJson(QJsonDocument::Compact)});

    operations->deleteLater();
}

void SIWebSocketProtocolV1::writePropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyWriteOperation*>(sender());
    emit frameReadyToSend({SIWebSocketProtocolFrame::PROPERTY_WRITTEN, {
        {"status", to_string(status)},
        {"id", operation->id().toString()}
    }});
    operation->deleteLater();
}
