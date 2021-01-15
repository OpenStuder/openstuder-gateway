#include "siabstractcommand.h"
#include <QCoreApplication>
#include <QTextStream>

void usage(QTextStream& output) {
    output << "Usage: sigwctl <command> ..." << endl << endl;
    output << "available commands:" << endl;
    for (const auto& command: SIAbstractCommand::allCommands().keys()) {
        output << "  " << command << endl;
    }
    output << endl << "use sigwctl help <command> to get help for a specific command." << endl;
}

int main(int argc, char** argv) {
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setOrganizationName("openstuder");
    QCoreApplication::setApplicationName("sigwctl");

    QCoreApplication application(argc, argv);
    QTextStream input(stdin, QIODevice::ReadOnly);
    QTextStream output(stdout, QIODevice::WriteOnly);

    if (QCoreApplication::arguments().count() < 2) {
        usage(output);
        return 1;
    }

    auto command = SIAbstractCommand::command(QCoreApplication::arguments()[1]);
    if (command != nullptr) {
        auto arguments = QCoreApplication::arguments();
        arguments.removeFirst();
        arguments.removeFirst();
        return command->run(arguments, input, output);
    } else {
        usage(output);
        return 1;
    }
}
