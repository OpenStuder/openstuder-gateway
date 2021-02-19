#pragma once
#include "sipropertyid.h"
#include <QString>
#include <memory>

class SIGlobalPropertyID {
  public:
    SIGlobalPropertyID();
    SIGlobalPropertyID(const char* uniqueID);
    SIGlobalPropertyID(const QByteArray& uniqueID);
    SIGlobalPropertyID(const QString& uniqueID);
    SIGlobalPropertyID(QString accessID, QString deviceID, SIPropertyID propertyID);

    bool isValid() const;

    const QString& accessID() const;

    const QString& deviceID() const;

    SIPropertyID propertyID() const;

    QString toString() const;

    bool isWildcard() const;
    bool matches(const SIGlobalPropertyID& id) const;
    bool matches(const QString& accessID, const QString& deviceID, SIPropertyID propertyID) const;

    bool operator ==(const SIGlobalPropertyID& other) const;
    bool operator <(const SIGlobalPropertyID& other) const;

  private:
    struct Private_;
    std::shared_ptr<Private_> private_;
};
