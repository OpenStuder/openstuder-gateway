#pragma once
#include <sipropertyid.h>
#include <QtCore/QString>

class SIGlobalPropertyID {
  public:
    SIGlobalPropertyID() = default;
    SIGlobalPropertyID(const QString& uniqueID);
    SIGlobalPropertyID(QString accessID, QString deviceID, SIPropertyID propertyID);

    bool isValid() const;

    const QString& accessID() const {
        return accessID_;
    }

    const QString& deviceID() const {
        return deviceID_;
    }

    SIPropertyID propertyID() const {
        return propertyID_;
    }

  private:
    QString accessID_;
    QString deviceID_;
    SIPropertyID propertyID_ = 0;
};
