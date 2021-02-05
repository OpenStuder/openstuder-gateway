#pragma once
#include <QString>

struct SIDeviceMessage {
    QString accessID;
    QString deviceID;
    quint64 messageID;
    QString message;
};
