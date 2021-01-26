#include "siabstractcommand.h"

class SIHelpCommand: public SIAbstractCommand {
  public:
    SIHelpCommand(): SIAbstractCommand("help") {}

  private:
    int run(QStringList arguments, QTextStream& input, QTextStream& output, const QVariantMap& options) override {
        Q_UNUSED(input)
        Q_UNUSED(options)

        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }

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
        for (const auto& command: SIAbstractCommand::allCommands().keys()) {
            output << "  " << command << endl;
        }
    }
};

static SIHelpCommand help;
