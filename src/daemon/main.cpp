#include "sidaemon.h"

int main(int argc, char** argv) {
    SIDaemon daemon {argc, argv};

    if (!daemon.initialize()) {
        return 1;
    }

    return SIDaemon::exec();
}
