#include "siextensionwebsocketresult.h"

SIExtensionWebSocketResult::SIExtensionWebSocketResult(): status_(SIExtensionStatus::Pending) {}

SIExtensionWebSocketResult::SIExtensionWebSocketResult(SIExtensionStatus status, std::initializer_list<QPair<QString, QString>> headers, QByteArray body)
: status_(status), body_(body) {
    for (const auto& header: headers) {
        headers_[header.first] = header.second;
    }

    if (status != SIExtensionStatus::Pending) {
        deleteLater();
    }
}

SIExtensionWebSocketResult* SIExtensionWebSocketResult::fromStatus(SIExtensionStatus status) {
    return new SIExtensionWebSocketResult(status);
}

void SIExtensionWebSocketResult::complete(SIExtensionStatus status, std::initializer_list<QPair<QString, QString>> headers, QByteArray body) {
    if (status_ != SIExtensionStatus::Pending || status == SIExtensionStatus::Pending) return;

    status_  = status;
    for (const auto& header: headers) {
        headers_[header.first] = header.second;
    }
    body_ = body;

    emit completed();
    deleteLater();
}
