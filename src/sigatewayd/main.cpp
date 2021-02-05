#include "sidaemon.h"
#include <csignal>

void signalHandler(int signal) {
    Q_UNUSED(signal)
    QCoreApplication::quit();
}

int main(int argc, char** argv) {

    // Install signal handlers to gracefully shut down daemon.
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // Create and initialize daemon.
    SIDaemon daemon(argc, argv);
    if (!daemon.initialize()) {
        return 1;
    }

    // Run the daemon's infinite event loop.
    return SIDaemon::exec();
}
