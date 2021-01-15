#include "siabstractcommand.h"

class SIHelpCommand: public SIAbstractCommand {
  public:
    SIHelpCommand(): SIAbstractCommand("help") {}

  private:
    int run(const QStringList& arguments, QTextStream& input, QTextStream& output) override {
        Q_UNUSED(input)

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
        output << "Usage: sigwctl help <command>" << endl;
    }
};

static SIHelpCommand help;