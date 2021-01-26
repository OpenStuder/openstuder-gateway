#include "siabstractcommand.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QCommandLineParser>

#include <QtDebug>

void usage(QTextStream& output) {
    output << "Usage: sigwctl [options] <command> ..." << endl << endl;
    output << "available commands:" << endl;
    for (const auto& command: SIAbstractCommand::allCommands().keys()) {
        output << "  " << command << endl;
    }
    output << endl << "available options:" << endl << "  -c, --config <dir>  Use configuration location <dir>" << endl;
    output << endl << "use sigwctl help <command> to get help for a specific command." << endl;
}

int main(int argc, char** argv) {
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setOrganizationName("openstuder");
    QCoreApplication::setApplicationName("sigwctl");

    QCoreApplication application(argc, argv);
    QTextStream input(stdin, QIODevice::ReadOnly);
    QTextStream output(stdout, QIODevice::WriteOnly);

    QCommandLineParser commandLineParser;
    commandLineParser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);

    QCommandLineOption configurationLocationOption {{"c", "config"}, "Use configuration location <dir>", "dir"};
    commandLineParser.addOption(configurationLocationOption);

    if (!commandLineParser.parse(QCoreApplication::arguments())) {
        usage(output);
        return 1;
    }

    if (commandLineParser.positionalArguments().count() < 1) {
        usage(output);
        return 1;
    }

    // Options.
    QVariantMap options;
    if (commandLineParser.isSet(configurationLocationOption)) {
        options["c"] = commandLineParser.value(configurationLocationOption);
    }

    auto command = SIAbstractCommand::command(commandLineParser.positionalArguments().first());
    if (command != nullptr) {
        auto arguments = commandLineParser.positionalArguments();
        arguments.removeFirst();
        return command->run(arguments, input, output, options);
    } else {
        usage(output);
        return 1;
    }
}
