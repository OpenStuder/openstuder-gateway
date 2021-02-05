#pragma once
#include <QIODevice>
#include <QMap>
#include <siglobalpropertyid.h>

class SIDataLogConfiguration {
  public:
    static SIDataLogConfiguration parse(QIODevice& device);

    inline bool valid() const {
        return valid_;
    }

    inline const QMap<SIGlobalPropertyID, int>& properties() const {
        return properties_;
    }

  private:
    bool valid_ = false;
    QMap<SIGlobalPropertyID ,int> properties_;
};
