#pragma once
#include <QStringList>
#include <QTextStream>
#include <QString>
#include <QMap>

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define endl Qt::endl
#else
#include <ostream>
#endif

class SIAbstractCommand {
  public:
    virtual ~SIAbstractCommand() = default;

    virtual int run(const QStringList& arguments, QTextStream& input, QTextStream& output) = 0;
    virtual void usage(QTextStream& output) = 0;

    static SIAbstractCommand* command(const QString& name);

    static const QMap<QString,SIAbstractCommand*>& allCommands();

  protected:
    explicit SIAbstractCommand(const QString& name);

  private:
    static QMap<QString,SIAbstractCommand*> commands_;
};
