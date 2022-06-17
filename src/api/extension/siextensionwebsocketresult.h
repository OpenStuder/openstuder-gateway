#pragma once
#include "siextensionstatus.h"
#include <QObject>
#include <QMap>
#include <QByteArray>

class SIExtensionWebSocketResult: public QObject {
    Q_OBJECT

    friend class SIExtension;

  public:
    SIExtensionWebSocketResult();
    SIExtensionWebSocketResult(SIExtensionStatus status, std::initializer_list<QPair<QString, QString>> headers = {}, QByteArray body = {});

    static SIExtensionWebSocketResult* fromStatus(SIExtensionStatus status);

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

    inline const QMap<QString, QString>& headers() const {
        return headers_;
    }

    inline const QByteArray& body() const {
        return body_;
    }

    void complete(SIExtensionStatus status, std::initializer_list<QPair<QString, QString>> headers = {}, QByteArray body = {});

  signals:
    void completed();

  private:
    QString extension_;
    QString command_;
    SIExtensionStatus status_;
    QMap<QString, QString> headers_;
    QByteArray body_;
};
