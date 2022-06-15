#pragma once
#include <siextension.h>
#include <QSqlDatabase>

class WebDataExtension: public SIExtension {
    Q_OBJECT

  public:
    WebDataExtension();
    ~WebDataExtension() override;

    bool open(const QString& filename);

  private:
    Result write_(const QString& key, const QByteArray& body);
    Result read_(const QString&  key);

    QStringList& commands_() const override;
    Result runCommand_(const QString& command, const QMap<QString, QString>& headers, const QByteArray& body) override;

    QSqlDatabase db_;
};
