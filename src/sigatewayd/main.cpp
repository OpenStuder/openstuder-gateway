#include "sidaemon.h"
#include <csignal>

int main(int argc, char** argv) {
    signal(SIGINT, [](int) {
        QCoreApplication::quit();
    });

    SIDaemon daemon(argc, argv);

    if (!daemon.initialize()) {
        return 1;
    }

    return SIDaemon::exec();
}
