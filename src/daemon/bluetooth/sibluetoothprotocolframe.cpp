#include "sibluetoothprotocolframe.h"
#include <QtDebug>

SIBluetoothProtocolFrame::SIBluetoothProtocolFrame(SIBluetoothProtocolFrame::Command command, const QVector<QString>& parameters): command_(command), parameters_(parameters) {}

bool SIBluetoothProtocolFrame::isNull() const {
    return command_ == INVALID;
}

QByteArray SIBluetoothProtocolFrame::toBytes() const {
    QByteArray frame(1, (quint8)command_);
    for (const auto& parameter: parameters_) {
        frame.append(" ");
        frame.append(parameter);
    }
    return frame;
}

SIBluetoothProtocolFrame SIBluetoothProtocolFrame::fromBytes(const QByteArray& bytes) {
    SIBluetoothProtocolFrame frame;

    if (bytes.count() >= 1) {
        frame.command_ = static_cast<Command>(bytes[0]);
        QString parameters = bytes.mid(1).trimmed();
        for (const auto& parameter: parameters.split(' ')) {
            frame.parameters_.append(parameter);
        }
    }

    return frame;
}

SIBluetoothProtocolFrame::SIBluetoothProtocolFrame(): command_(ERROR) {}
