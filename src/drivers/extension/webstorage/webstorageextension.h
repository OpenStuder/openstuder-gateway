#pragma once
#include <siextension.h>
#include <QSqlDatabase>

class WebStorageExtension: public SIExtension {
    Q_OBJECT

  public:
    WebStorageExtension();
    ~WebStorageExtension() override;

    bool open(const QString& filename);

  private:
    SIExtensionWebSocketResult* write_(const QString& key, const QByteArray& body);
    SIExtensionWebSocketResult* read_(const QString&  key);

    QStringList& commands_() const override;
    SIExtensionWebSocketResult* runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) override;

    QSqlDatabase db_;
};
