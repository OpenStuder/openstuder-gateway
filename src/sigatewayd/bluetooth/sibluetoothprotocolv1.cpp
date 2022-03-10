#include "sibluetoothprotocolv1.h"
#include <sistorage.h>
#include <sideviceaccessregistry.h>

SIBluetoothProtocolV1::SIBluetoothProtocolV1(SIAccessLevel accessLevel): accessLevel_(accessLevel) {}

SIBluetoothProtocolFrame SIBluetoothProtocolV1::handleFrame(SIBluetoothProtocolFrame& frame, SIContext& context) {
    switch (frame.command()) {
        case SIBluetoothProtocolFrame::AUTHORIZE:
            return {SIBluetoothProtocolFrame::ERROR, {"invalid state"}};

        case SIBluetoothProtocolFrame::ENUMERATE: {
            if (frame.parameterCount() != 0) { return SIBluetoothProtocolFrame::error("invalid frame"); }

            auto* operation = context.deviceAccessManager().enumerateDevices();
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::enumerationOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::DESCRIBE: {
            if (!frame.validateParameters({{QVariant::String, QVariant::Invalid}})) {
                return SIBluetoothProtocolFrame::error("invalid frame");
            }

            if (frame.parameters()[0].canConvert<QString>()) {
                auto id = frame.parameters()[0].toString().split(".");
                switch (id.count()) {
                    case 1: {
                        auto deviceAccess = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(id[0]);
                        if (deviceAccess == nullptr) {
                            return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::NoDeviceAccess, frame.parameters()[0], QVariant {}}};
                        }

                        QVariantMap devices;
                        for (int i = 0; i < deviceAccess->deviceCount(); ++i) {
                            auto device = deviceAccess->device(i);
                            devices[device->id()] = device->model();
                        }
                        return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::Success, frame.parameters()[0], devices}};
                    }

                    case 2: {
                        auto deviceAccess = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(id[0]);
                        if (deviceAccess == nullptr) {
                            if (deviceAccess == nullptr) {
                                return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::NoDeviceAccess, frame.parameters()[0], QVariant {}}};
                            }
                        }

                        auto device = deviceAccess->device(id[1]);
                        if (device == nullptr) {
                            return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::NoDevice, frame.parameters()[0], QVariant {}}};
                        }

                        QList<QVariant> properties;
                        for (const auto& property: device->properties()) {
                            properties << property.id();
                        }


                        return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::Success, frame.parameters()[0], properties}};
                    }

                    case 3: {
                        auto property = context.deviceAccessManager().resolveProperty(SIGlobalPropertyID(frame.parameters()[0].toString()));
                        if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                            return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::NoProperty, frame.parameters()[0], QVariant {}}};
                        }

                        QVariantMap description;
                        description["type"] = static_cast<int>(property.type());
                        description["flags"] = static_cast<unsigned int>(property.flags());
                        description["description"] = property.description();
                        if (property.type() == SIPropertyType::Enum) {
                            description["values"] = property.enumValues();
                        } else {
                            description["unit"] = property.unit();
                        }
                        return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::Success, frame.parameters()[0], description}};
                    }

                    default:
                        return SIBluetoothProtocolFrame::error("invalid frame");
                }
            } else {
                QVariantMap accesses;
                for (int i = 0; i < SIDeviceAccessRegistry::sharedRegistry().deviceAccessCount(); ++i) {
                    auto access = SIDeviceAccessRegistry::sharedRegistry().deviceAccess(i);
                    accesses[access->id()] = SIDeviceAccessRegistry::sharedRegistry().driverName(access);
                }
                return {SIBluetoothProtocolFrame::DESCRIPTION, {(int)SIStatus::Success, frame.parameters()[0], accesses}};
            }
        }

        case SIBluetoothProtocolFrame::READ_PROPERTY: {
            if (!frame.validateParameters({{QVariant::String}})) {
                return SIBluetoothProtocolFrame::error("invalid frame");
            }

            SIGlobalPropertyID id(frame.parameters()[0].value<QString>());
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIBluetoothProtocolFrame::PROPERTY_READ, {(int)SIStatus::NoProperty, frame.parameters()[0], {}}};
            }
            if (!property.isFlagSet(SIPropertyFlag::Readable)) {
                return {SIBluetoothProtocolFrame::PROPERTY_READ, {(int)SIStatus::Error, frame.parameters()[0], {}}};
            }

            auto* operation = context.deviceAccessManager().readProperty(id);
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::readPropertyOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::WRITE_PROPERTY: {
            if (!frame.validateParameters({
                                              {QVariant::String},
                                              {QVariant::Int, QVariant::Invalid},
                                              {}
                                          })) {
                return SIBluetoothProtocolFrame::error("invalid frame");
            }

            SIGlobalPropertyID id(frame.parameters()[0].value<QString>());
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIBluetoothProtocolFrame::PROPERTY_WRITTEN, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }
            if (!property.isFlagSet(SIPropertyFlag::Writeable)) {
                return {SIBluetoothProtocolFrame::PROPERTY_WRITTEN, {(int)SIStatus::Error, frame.parameters()[0]}};
            }

            QVariant value = frame.parameters()[2];

            SIPropertyWriteFlags writeFlags = SIPropertyWriteFlag::Default;
            if (frame.parameters()[1].canConvert<unsigned int>()) {
                writeFlags = static_cast<SIPropertyWriteFlags>(frame.parameters()[1].value<unsigned int>());
            }

            auto* operation = context.deviceAccessManager().writeProperty(id, value, writeFlags);
            connect(operation, &SIAbstractOperation::finished, this, &SIBluetoothProtocolV1::writePropertyOperationFinished_);
            return {};
        }

        case SIBluetoothProtocolFrame::SUBSCRIBE_PROPERTY: {
            if (!frame.validateParameters({{QVariant::String}})) { return SIBluetoothProtocolFrame::error("invalid frame"); }

            SIGlobalPropertyID id(frame.parameters()[0].value<QString>());
            auto property = context.deviceAccessManager().resolveProperty(id);
            if (property.type() == SIPropertyType::Invalid || accessLevel_ < property.accessLevel()) {
                return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }
            if (!property.isFlagSet(SIPropertyFlag::Readable)) {
                return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {(int)SIStatus::Error, frame.parameters()[0]}};
            }

            bool status = context.deviceAccessManager().subscribeToProperty(id, this);
            return {SIBluetoothProtocolFrame::PROPERTY_SUBSCRIBED, {(int)(status ? SIStatus::Success : SIStatus::Error), frame.parameters()[0]}};
        }

        case SIBluetoothProtocolFrame::UNSUBSCRIBE_PROPERTY: {
            if (!frame.validateParameters({{QVariant::String}})) { return SIBluetoothProtocolFrame::error("invalid frame"); }

            auto id = SIGlobalPropertyID(frame.parameters()[0].toString());
            if (!id.isValid()) {
                return {SIBluetoothProtocolFrame::PROPERTY_UNSUBSCRIBED, {(int)SIStatus::NoProperty, frame.parameters()[0]}};
            }

            bool status = context.deviceAccessManager().unsubscribeFromProperty(id, this);
            return {SIBluetoothProtocolFrame::PROPERTY_UNSUBSCRIBED, {(int)(status ? SIStatus::Success : SIStatus::Error), frame.parameters()[0]}};
        }

        case SIBluetoothProtocolFrame::READ_DATALOG: {
            if (!frame.validateParameters({
                {QVariant::String, QVariant::Invalid},
                {QVariant::ULongLong, QVariant::Invalid},
                {QVariant::ULongLong, QVariant::Invalid},
                {QVariant::UInt, QVariant::Invalid}
            })) { return SIBluetoothProtocolFrame::error("invalid frame"); }


            auto id = SIGlobalPropertyID();
            if (!frame.parameters()[0].isNull()) {
                id = SIGlobalPropertyID(frame.parameters()[0].toString());
                if (!id.isValid()) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            auto from = QDateTime::fromSecsSinceEpoch(0, Qt::UTC);
            if (!frame.parameters()[1].isNull()) {
                from = QDateTime::fromSecsSinceEpoch(frame.parameters()[1].toULongLong(), Qt::UTC);
                if (!from.isValid()) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            auto to = QDateTime::currentDateTimeUtc();
            if (!frame.parameters()[2].isNull()) {
                to = QDateTime::fromSecsSinceEpoch(frame.parameters()[2].toULongLong(), Qt::UTC);
                if (!from.isValid()) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            auto limit = 1000U;
            if (!frame.parameters()[3].isNull()) {
                bool conversionOk = false;
                limit = qMin(1000U, frame.parameters()[3].toUInt(&conversionOk));
                if (!conversionOk) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            if (id.isValid()) {
                SIStatus status;
                auto data = context.storage().retrievePropertyValues(id, from, to, limit, &status);

                QList<QVariant> values;
                for (auto entry = data.crbegin(); entry != data.crend(); ++entry) {
                    values << entry->timestamp.toUTC().toSecsSinceEpoch() << entry->value;
                }

                return {SIBluetoothProtocolFrame::DATALOG_READ, {(int)status, id.toString(), data.count(), values}};
            } else {
                SIStatus status;
                auto storedPropertyIDs = context.storage().availableStoredProperties(from, to, &status);

                QStringList ids;
                for (const auto & propertyID : storedPropertyIDs) {
                    ids << propertyID.toString();
                }

                return {SIBluetoothProtocolFrame::DATALOG_READ, {(int)status, {}, storedPropertyIDs.count(), ids}};
            }
        }

        case SIBluetoothProtocolFrame::READ_MESSAGES: {
            if (!frame.validateParameters({
                {QVariant::ULongLong, QVariant::Invalid},
                {QVariant::ULongLong, QVariant::Invalid},
                {QVariant::UInt, QVariant::Invalid}
            })) {
                return SIBluetoothProtocolFrame::error("invalid frame");
            }

            auto from = QDateTime::fromSecsSinceEpoch(0, Qt::UTC);
            if (!frame.parameters()[0].isNull()) {
                from = QDateTime::fromSecsSinceEpoch(frame.parameters()[0].toULongLong(), Qt::UTC);
                if (!from.isValid()) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            auto to = QDateTime::currentDateTimeUtc();
            if (!frame.parameters()[1].isNull()) {
                to = QDateTime::fromSecsSinceEpoch(frame.parameters()[1].toULongLong(), Qt::UTC);
                if (!from.isValid()) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            auto limit = std::numeric_limits<unsigned int>::max();
            if (!frame.parameters()[2].isNull()) {
                bool conversionOk = false;
                limit = frame.parameters()[2].toUInt(&conversionOk);
                if (!conversionOk) {
                    return SIBluetoothProtocolFrame::error("invalid frame");
                }
            }

            SIStatus status;
            auto messages = context.storage().retrieveDeviceMessages(from, to, limit, &status);

            QList<QVariant> messageList;
            for (auto message = messages.crbegin(); message != messages.crend(); ++message) {
                messageList << message->timestamp().toUTC().toSecsSinceEpoch()
                            << message->accessID()
                            << message->deviceID()
                            << message->messageID()
                            << message->message();
            }

            return {SIBluetoothProtocolFrame::MESSAGES_READ, {(int)status, messages.count(), messageList}};
        }

        case SIBluetoothProtocolFrame::FIND_PROPERTIES: {
            if (!frame.validateParameters({{QVariant::String}})) { return SIBluetoothProtocolFrame::error("invalid frame"); }

            auto id = SIGlobalPropertyID(frame.parameters()[0].toString());
            if (!id.isValid()) {
                return SIBluetoothProtocolFrame::error("invalid frame");
            }

            auto propertyIDs = context.deviceAccessManager().findProperties(id);
            QVariantList propertyIDStrings;
            for (const auto& id: propertyIDs) propertyIDStrings << id.toString();

            return {SIBluetoothProtocolFrame::PROPERTIES_FOUND, {(int)SIStatus::Success, frame.parameters()[0], propertyIDs.count(), propertyIDStrings}};
        }

        default:
            return {SIBluetoothProtocolFrame::ERROR, {"invalid frame"}};
    }
}

SIBluetoothProtocolFrame SIBluetoothProtocolV1::convertDeviceMessage(const SIDeviceMessage& message) {
    return {SIBluetoothProtocolFrame::DEVICE_MESSAGE, {message.timestamp().toUTC().toSecsSinceEpoch(), message.accessID(), message.deviceID(), message.messageID(), message.message()}};
}

void SIBluetoothProtocolV1::propertyChanged(SIGlobalPropertyID id, const QVariant& value) {
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_UPDATE, {id.toString(), value}});
}

void SIBluetoothProtocolV1::enumerationOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIDeviceEnumerationOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::ENUMERATED, {(int)status, operation->numberOfDevicesPresent()}});
    operation->deleteLater();
}

void SIBluetoothProtocolV1::readPropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyReadOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_READ, {(int)status, operation->id().toString(), operation->value()}});
    operation->deleteLater();
}

void SIBluetoothProtocolV1::writePropertyOperationFinished_(SIStatus status) {
    auto* operation = dynamic_cast<SIPropertyWriteOperation*>(sender());
    emit frameReadyToSend({SIBluetoothProtocolFrame::PROPERTY_WRITTEN, {(int)status, operation->id().toString()}});
    operation->deleteLater();
}
