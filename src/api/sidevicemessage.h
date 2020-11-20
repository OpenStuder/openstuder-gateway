#pragma once
#include <QString>

struct SIDeviceMessage {
    QString deviceID;
    quint64 messageID;
    QString message;
};
