#pragma once
#include <QIODevice>
#include <QMap>
#include <QVector>
#include <siglobalpropertyid.h>

class SIDataLogConfiguration {
  public:
    static SIDataLogConfiguration parse(QIODevice& device);

    inline bool valid() const {
        return valid_;
    }

    inline const QMap<int, QVector<SIGlobalPropertyID>>& properties() const {
        return properties_;
    }

  private:
    bool valid_ = false;
    QMap<int,QVector<SIGlobalPropertyID>> properties_;
};
