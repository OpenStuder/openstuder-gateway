#pragma once
#include <QStringList>
#include <QTextStream>
#include <QString>
#include <QMap>
#include <QVariantMap>

// Fix compilation warning on Qt 5.15 and newer.
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define endl Qt::endl
#else
#include <ostream>
#endif

/**
 * @brief Command interface.
 *
 * All commands supported by sigwctl have to derive from this and a static instance has to be created in order to register the commad.
 */
class SIAbstractCommand {
  public:
    virtual ~SIAbstractCommand() = default;

    /**
     * @brief This method is called by the main function in order to actually run the command.
     *
     * All arguments and command line options are passed to the command, the command can use the input and output streams to interact with the user and should return the status.
     *
     * @param arguments Input arguments for the command. The command has to check itself if the count of arguments is correct.
     * @param input     Input stream, can be used by the command to read from the console.
     * @param output    Output stream (stdout) to write to the console.
     * @param options   Named options that were passed in front of the command when calling sigwctl.
     * @return          The command implementation should return 0 on success and any other value (typical 1) on error.
     */
    virtual int run(QStringList arguments, QTextStream& input, QTextStream& output, const QVariantMap& options) = 0;

    /**
     * @brief This method is called to show how to use the command on the console.
     *
     * @param output    Output stream to which the usage information has to be written.
     */
    virtual void usage(QTextStream& output) = 0;

    /**
     * @brief Returns the command with the given name or nullptr if no command with that name exists.
     *
     * @param name  Name of the command.
     * @return      Pointer to the command or nullptr if the command does not exist.
     */
    static SIAbstractCommand* command(const QString& name);

    /**
     * @brief Returns the list of all command names.
     *
     * @return  List of all existing commands.
     */
    static QStringList commandNames();

  protected:

    /**
     * @brief The command is registered with the name passed in the constructor.
     *
     * @param name  Name of the command.
     */
    explicit SIAbstractCommand(const QString& name);

  private:

    // List of all registered commands.
    static QMap<QString,SIAbstractCommand*> commands_;
};
