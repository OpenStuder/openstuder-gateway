#pragma once
#include "siextensionstatus.h"
#include <QObject>
#include <QVector>
#include <QVariant>

class SIExtensionBluetoothResult: public QObject {
    Q_OBJECT

    friend class SIExtension;

  public:
    SIExtensionBluetoothResult();
    SIExtensionBluetoothResult(SIExtensionStatus status, std::initializer_list<QVariant> results = {});
    SIExtensionBluetoothResult(SIExtensionStatus status, QVector<QVariant> results);

    static SIExtensionBluetoothResult* fromStatus(SIExtensionStatus status);

    inline bool isComplete() const {
        return status_  != SIExtensionStatus::Pending;
    }

    inline const QString& extension() const {
        return extension_;
    }

    inline const QString& command() const {
        return command_;
    }

    inline SIExtensionStatus status() const {
        return status_;
    }

    inline const QVector<QVariant>& results() const {
        return results_;
    }

    void complete(SIExtensionStatus status, std::initializer_list<QVariant> results = {});
    void complete(SIExtensionStatus status, QVector<QVariant> results);

  signals:
    void completed();

  private:
    QString extension_;
    QString command_;
    SIExtensionStatus status_;
    QVector<QVariant> results_;
};
