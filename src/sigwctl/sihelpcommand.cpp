#include "siabstractcommand.h"

class SIHelpCommand: public SIAbstractCommand {
  public:
    SIHelpCommand(): SIAbstractCommand("help") {}

  private:
    int run(QStringList arguments, QTextStream& input, QTextStream& output, const QVariantMap& options) override {
        Q_UNUSED(input)
        Q_UNUSED(options)

        // The help command needs at least one argument.
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }

        // Retrieve command and print the commands usage if found, print error output else.
        auto command = SIAbstractCommand::command(arguments[0]);
        if (command != nullptr) {
            command->usage(output);
            return 0;
        } else {
            output << "Command \"" << arguments[0] << "\" does not exist." << endl;
            return 1;
        }
    }

    void usage(QTextStream& output) override {
        output << "Usage: sigwctl help <command>" << endl << endl;
        output << "<command>:" << endl;
        for (const auto& command: SIAbstractCommand::commandNames()) {
            output << "  " << command << endl;
        }
    }
};

static SIHelpCommand help;
