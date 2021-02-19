#include "siabstractcommand.h"

class SIVersionCommand: public SIAbstractCommand {
  public:
    SIVersionCommand(): SIAbstractCommand("version") {}

    int run(QStringList arguments, QTextStream& input, QTextStream& output, const QVariantMap& options) override {
        Q_UNUSED(input)
        Q_UNUSED(options)

        if (!arguments.empty()) {
            usage(output);
            return 1;
        }

        output << "OpenStuder gateway, version " << OPENSTUDER_GATEWAY_VERSION << endl;

        return 0;
    }

    void usage(QTextStream& output) override {
       output << "Usage: sigwctl version" << endl;
    }
};

static SIVersionCommand siVersionCommand; // NOLINT(cert-err58-cpp)
