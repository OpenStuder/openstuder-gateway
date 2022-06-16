#pragma once
#include "siextension.h"
#include "../sicontext.h"
#include "sisessioncontext.h"
#include <QObject>
#include <QMap>

class SIExtensionManager: public QObject {
  public:
    QStringList availableExtensions() const;

    SIExtension::Result callExtension(const QString& extension, const QString& command,
                                      const QMap<QString,QString>& headers, const QByteArray& body, SIContext& context, SISessionContext& sessionCtx);

    void addExtension(SIExtension* extension);

  private:
    class Context;

    QMap<QString,SIExtension*> extensions_;
};
