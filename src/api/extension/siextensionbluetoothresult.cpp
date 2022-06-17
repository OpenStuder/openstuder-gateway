#include "siextensionbluetoothresult.h"

SIExtensionBluetoothResult::SIExtensionBluetoothResult(): status_(SIExtensionStatus::Pending) {}

SIExtensionBluetoothResult::SIExtensionBluetoothResult(SIExtensionStatus status, std::initializer_list<QVariant> results): status_(status), results_(results) {
    if (status != SIExtensionStatus::Pending) {
        deleteLater();
    }
}

SIExtensionBluetoothResult::SIExtensionBluetoothResult(SIExtensionStatus status, QVector<QVariant> results): status_(status), results_(std::move(results)) {
    if (status != SIExtensionStatus::Pending) {
        deleteLater();
    }
}

SIExtensionBluetoothResult* SIExtensionBluetoothResult::fromStatus(SIExtensionStatus status) {
    return new SIExtensionBluetoothResult(status);
}

void SIExtensionBluetoothResult::complete(SIExtensionStatus status, std::initializer_list<QVariant> results) {
    complete(status, QVector<QVariant>(results));
}

void SIExtensionBluetoothResult::complete(SIExtensionStatus status, QVector<QVariant> results) {
    if (status_ != SIExtensionStatus::Pending || status == SIExtensionStatus::Pending) return;

    status_  = status;
    results_ = results;

    emit completed();
    deleteLater();
}
