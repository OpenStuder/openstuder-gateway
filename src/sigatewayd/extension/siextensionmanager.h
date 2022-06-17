#pragma once
#include "siextension.h"
#include "../sicontext.h"
#include "sisessioncontext.h"
#include <functional>
#include <QObject>
#include <QMap>

class SIExtensionManager: public QObject {
  public:
    QStringList availableExtensions(const std::function<bool(const SIExtension*)>& predicate = nullptr) const;

    const SIExtensionWebSocketResult* callExtension(const QString& extension, const QString& command,
                                                    const QMap<QString, QString>& headers, const QByteArray& body, SIContext& context, SISessionContext& sessionCtx);

    const SIExtensionBluetoothResult* callExtension(const QString& extension, const QString& command,
                                                    const QVector<QVariant>& parameters, SIContext& context, SISessionContext& sessionCtx);

    void addExtension(SIExtension* extension);

  private:
    class Context_;

    QMap<QString, SIExtension*> extensions_;
};
