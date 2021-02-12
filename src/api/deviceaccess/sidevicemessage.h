#pragma once
#include <QString>
#include <QDateTime>
#include <algorithm>

struct SIDeviceMessage {
    inline SIDeviceMessage(): timestamp(QDateTime::currentDateTime()), messageID(0) {}

    inline SIDeviceMessage(QString accessID, QString deviceID, quint64 messageID, QString message, QDateTime timestamp = QDateTime::currentDateTime())
        : timestamp(std::move(timestamp)), accessID(std::move(accessID)), deviceID(std::move(deviceID)), messageID(messageID), message(std::move(message)) {}

    QDateTime timestamp;
    QString accessID;
    QString deviceID;
    quint64 messageID;
    QString message;
};
