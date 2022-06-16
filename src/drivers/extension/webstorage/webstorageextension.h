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
    Result write_(const QString& key, const QByteArray& body);
    Result read_(const QString&  key);

    QStringList& commands_() const override;
    Result runCommand_(const SIExtensionContext& context, const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) override;

    QSqlDatabase db_;
};
