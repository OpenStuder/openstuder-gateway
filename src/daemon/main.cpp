#include "sidaemon.h"
#include <csignal>

int main(int argc, char** argv) {
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setOrganizationName("openstuder");
    QCoreApplication::setApplicationName("sigatewayd");

    signal(SIGINT, [](int) {
        QCoreApplication::quit();
    });

    SIDaemon daemon(argc, argv);

    if (!daemon.initialize()) {
        return 1;
    }

    return SIDaemon::exec();
}
