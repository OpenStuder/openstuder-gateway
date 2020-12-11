#pragma once
#include <sipropertyid.h>
#include <QtCore/QString>

class SIGlobalPropertyID {
  public:
    inline SIGlobalPropertyID() = default;
    inline SIGlobalPropertyID(const char* uniqueID): SIGlobalPropertyID(QString(uniqueID)) {}
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

    bool operator ==(const SIGlobalPropertyID& other) const;

  private:
    QString accessID_;
    QString deviceID_;
    SIPropertyID propertyID_ = 0;
};
