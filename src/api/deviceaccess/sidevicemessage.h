#pragma once
#include <QString>
#include <QDateTime>
#include <algorithm>
#include <memory>

class SIDeviceMessage {
  public:
    SIDeviceMessage();

    SIDeviceMessage(QString accessID, QString deviceID, quint64 messageID, QString message, QDateTime timestamp = QDateTime::currentDateTime());

    bool isValid() const;

    const QDateTime& timestamp() const;

    const QString& accessID() const;

    void setAccessID(const QString& accessID);

    const QString& deviceID() const;

    quint64 messageID() const;

    const QString& message() const;

  private:
    void detach_();

    struct Private_;
    std::shared_ptr<Private_> private_;
};
