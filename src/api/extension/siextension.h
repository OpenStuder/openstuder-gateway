#pragma once
#include <memory>
#include <QObject>
#include <QString>
#include <QPair>
#include <QByteArray>
#include <QMap>

class SIExtension: public QObject {
  public:
    enum class Status: int {
        Success = 0,
        UnsupportedExtension = - 1,
        UnsupportedCommand = - 2,
        InvalidHeaders = - 3,
        InvalidBody = - 4,
        Error = - 5
    };

    class Result {
      public:
        inline Result(Status status, std::initializer_list<QPair<QString, QString>> headers = {}, QByteArray body = {}): status_(status), body_(std::move(body)) {
            for (const auto& header: headers) {
                headers_[header.first] = header.second;
            }
        }

        inline Status status() const {
            return status_;
        }

        inline const QMap<QString, QString>& headers() const {
            return headers_;
        }

        inline const QByteArray& body() const {
            return body_;
        }

      private:
        Status status_;
        QMap<QString, QString> headers_;
        QByteArray body_;
    };

    SIExtension(const SIExtension&) = delete;
    SIExtension& operator =(const SIExtension&) = delete;
    ~SIExtension() override;

    const QString& id() const;

    const QStringList& commands() const;

    Result runCommand(const QString& command, const QMap<QString, QString>& headers, const QByteArray& body);

  protected:
    explicit SIExtension(const QString& id);

  private:
    virtual QStringList& commands_() const = 0;

    virtual Result runCommand_(const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) = 0;

    struct Private_;
    std::unique_ptr<Private_> private_;
};

QString to_string(SIExtension::Status status);