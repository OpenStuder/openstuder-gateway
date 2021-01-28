#include "siabstractcommand.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QCommandLineParser>

void printUsage(QTextStream& output);

int main(int argc, char** argv) {
    // Setup basic application information.
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setOrganizationName("openstuder");
    QCoreApplication::setApplicationName("sigwctl");
    QCoreApplication::setApplicationVersion(OPENSTUDER_GATEWAY_VERSION);

    // Create application object and the input and output stream.
    QCoreApplication application(argc, argv);
    QTextStream input(stdin, QIODevice::ReadOnly);
    QTextStream output(stdout, QIODevice::WriteOnly);

    // Use QCommandLineParser to parse the options in front of the command.
    QCommandLineParser commandLineParser;
    commandLineParser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);

    // Allow the user to override the configuration directory.
    QCommandLineOption configurationLocationOption {{"c", "config"}, "Use configuration location <dir>", "dir"};
    commandLineParser.addOption(configurationLocationOption);

    // Try to parse the command line arguments and print usage if parsing fails.
    if (!commandLineParser.parse(QCoreApplication::arguments())) {
        printUsage(output);
        return 1;
    }

    // If the command is issued without specifying the command, print usage and quit.
    if (commandLineParser.positionalArguments().count() < 1) {
        printUsage(output);
        return 0;
    }

    // Create map of options that will be passed to the .
    QVariantMap options;
    if (commandLineParser.isSet(configurationLocationOption)) {
        options["c"] = commandLineParser.value(configurationLocationOption);
    }

    // Get the requested command from the command registry, if the command does not exist, show usage and quit.
    auto* command = SIAbstractCommand::command(commandLineParser.positionalArguments().first());
    if (command == nullptr) {
        printUsage(output);
        return 1;
    }

    // Remove the command itself from the arguments that get passed further.
    auto arguments = commandLineParser.positionalArguments();
    arguments.removeFirst();

    // Run the command and return it's status.
    return command->run(arguments, input, output, options);
}

void printUsage(QTextStream& output) {
    output << "Usage: sigwctl [options] <command> ..." << endl << endl;
    output << "available commands:" << endl;
    for (const auto& command: SIAbstractCommand::commandNames()) {
        output << "  " << command << endl;
    }
    output << endl << "available options:" << endl
           << "  -c, --config <dir>  Use configuration location <dir>" << endl;
    output << endl << "use sigwctl help <command> to get help for a specific command." << endl;
}
