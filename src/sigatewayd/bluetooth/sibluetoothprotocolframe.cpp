#include "sibluetoothprotocolframe.h"
#include <cbor.h>
#include <QDateTime>

SIBluetoothProtocolFrame::SIBluetoothProtocolFrame(SIBluetoothProtocolFrame::Command command, const QVector<QVariant>& parameters): command_(command), parameters_(parameters) {}

bool SIBluetoothProtocolFrame::isNull() const {
    return command_ == INVALID;
}

bool SIBluetoothProtocolFrame::validateParameters(const QVector<QSet<QVariant::Type>>& parameterTypes) {
    if (parameters_.count() != parameterTypes.count()) {
        return false;
    }

    for (int i = 0; i < parameters_.count(); ++i) {
        if (!parameterTypes[i].isEmpty() && std::none_of(parameterTypes[i].cbegin(), parameterTypes[i].cend(), [this, i](QVariant::Type type) { return parameters_[i].canConvert(type); })) {
            return false;
        }
    }

    return true;
}

QByteArray SIBluetoothProtocolFrame::toBytes(qsizetype bufferSize) const {
    QByteArray bytes(bufferSize, 0);
    CborEncoder encoder;
    cbor_encoder_init(&encoder, reinterpret_cast<uint8_t*>(bytes.data()), bytes.size(), 0);

    if (cbor_encode_uint(&encoder, command_) != CborNoError) {
        return {};
    }

    for (const auto& parameter: parameters_) {
        auto error = static_cast<CborError>(encodeVariant_(&encoder, parameter));
        switch (error) {
            case CborNoError:
                break;

            case CborErrorOutOfMemory:
                return toBytes(bufferSize * 8);

            default:
                return {};
        }
    }

    auto size = cbor_encoder_get_buffer_size(&encoder, reinterpret_cast<const uint8_t*>(bytes.data()));
    bytes.resize(size);

    return bytes;
}

SIBluetoothProtocolFrame SIBluetoothProtocolFrame::fromBytes(const QByteArray& bytes) {
    CborParser parser;
    CborValue it;

    if (cbor_parser_init(reinterpret_cast<const uint8_t*>(bytes.data()), bytes.size(), 0, &parser, &it) != CborNoError) {
        return {};
    }

    uint64_t command = 0;
    if (!cbor_value_is_unsigned_integer(&it) || cbor_value_get_uint64(&it, &command) != CborNoError) {
        return {};
    }
    cbor_value_advance_fixed(&it);

    QVector<QVariant> parameters;
    while (cbor_value_get_next_byte(&it) < parser.end) {
        cbor_parser_init(it.ptr, reinterpret_cast<const uint8_t*>(bytes.data()) + bytes.size() - it.ptr, 0, &parser, &it);
        CborError error = CborNoError;
        switch (cbor_value_get_type(&it)) {
            case CborNullType:
                parameters.append(QVariant {});
                error = cbor_value_advance_fixed(&it);
                break;

            case CborBooleanType: {
                bool value;
                error = cbor_value_get_boolean(&it, &value);
                if (error == CborNoError) {
                    parameters.append(value);
                    error = cbor_value_advance_fixed(&it);
                }
                break;
            }

            case CborIntegerType:
                if (cbor_value_is_unsigned_integer(&it)) {
                    uint64_t value;
                    error = cbor_value_get_uint64(&it, &value);
                    if (error == CborNoError) {
                        parameters.append(static_cast<quint64>(value));
                        error = cbor_value_advance_fixed(&it);
                    }
                } else {
                    int64_t value;
                    error = cbor_value_get_int64(&it, &value);
                    if (error == CborNoError) {
                        parameters.append(static_cast<qint64>(value));
                        error = cbor_value_advance_fixed(&it);
                    }
                }
                break;

            case CborDoubleType: {
                double value;
                error = cbor_value_get_double(&it, &value);
                if (error == CborNoError) {
                    parameters.append(value);
                    error = cbor_value_advance_fixed(&it);
                }
                break;
            }


            case CborTextStringType: {
                size_t length;
                error = cbor_value_get_string_length(&it, &length);
                if (error == CborNoError) {
                    QByteArray value(length, 0);
                    error = cbor_value_copy_text_string(&it, value.data(), &length, &it);
                    if (error == CborNoError) {
                        parameters.append(QString::fromUtf8(value));
                    }
                }
                break;
            }

            default:
                return {};
        }

        if (error != CborNoError) {
            return {};
        }
    }

    return {static_cast<Command>(command), parameters};
}

int SIBluetoothProtocolFrame::encodeVariant_(CborEncoder* encoder, const QVariant& variant) const {
    CborError error = CborNoError;
    switch (variant.type()) {
        case QVariant::Invalid:
            error = cbor_encode_null(encoder);
            break;

        case QVariant::Bool:
            error = cbor_encode_boolean(encoder, variant.toBool());
            break;

        case QVariant::Int:
        case QVariant::LongLong:
            error = cbor_encode_int(encoder, variant.toLongLong());
            break;

        case QVariant::UInt:
        case QVariant::ULongLong:
            error = cbor_encode_uint(encoder, variant.toULongLong());
            break;

        case QVariant::Double:
            error = cbor_encode_double(encoder, variant.toDouble());
            break;

        case QVariant::String: {
            auto text = variant.toString().toUtf8();
            error = cbor_encode_text_string(encoder, reinterpret_cast<const char*>(text.data()), text.size());
            break;
        }

        case QVariant::List:
        case QVariant::StringList: {
            auto list = variant.toList();
            CborEncoder arrayEncoder;
            error = cbor_encoder_create_array(encoder, &arrayEncoder, list.size());
            if (error != CborNoError) { break; }
            for (const auto& entry: list) {
                error = static_cast<CborError>(encodeVariant_(&arrayEncoder, entry));
                if (error != CborNoError) {
                    return error;
                }
            }
            error = cbor_encoder_close_container(encoder, &arrayEncoder);
            break;
        }

        case QVariant::Map: {
            auto map = variant.toMap();
            CborEncoder mapEncoder;
            error = cbor_encoder_create_map(encoder, &mapEncoder, map.count());
            if (error != CborNoError) { break; }
            for (const auto& entry: map.keys()) {
                auto key = entry.toUtf8();
                auto value = map[key];
                error = cbor_encode_text_string(&mapEncoder, key.data(), key.size());
                if (error != CborNoError) {
                    return error;
                }
                error = static_cast<CborError>(encodeVariant_(&mapEncoder, value));
                if (error != CborNoError) {
                    return error;
                }
            }
            error = cbor_encoder_close_container(encoder, &mapEncoder);
            break;
        }

        default:
            return CborErrorUnsupportedType;
    }

    return error;
}
