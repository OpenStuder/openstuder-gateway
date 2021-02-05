#pragma once
#include <QIODevice>
#include <QMap>
#include <siglobalpropertyid.h>

class SIDataLogConfiguration {
  public:
    static SIDataLogConfiguration parse(QIODevice& device);

  private:
    bool valid_ = false;
    QMap<SIGlobalPropertyID ,int> properties_;
};
