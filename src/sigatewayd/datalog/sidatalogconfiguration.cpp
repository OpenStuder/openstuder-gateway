#include "sidatalogconfiguration.h"

SIDataLogConfiguration SIDataLogConfiguration::parse(QIODevice& device) {
    SIDataLogConfiguration dataLogConfiguration;

    // If the device is not open yet, try to open it.
    if (!device.isOpen() && !device.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("could not open datalog configuration");
    }

    // Read  file line per line.
    int lineNumber = 0;
    int actualReadInterval = 0;
    while (!device.atEnd()) {
        lineNumber++;
        auto line = device.readLine();

        // Ignore comments.
        if (line.startsWith("#")) {
            continue;
        }

        // Handle read groups.
        if (line.startsWith("interval ")) {
            auto split = line.split(' ');
            bool ok = false;
            if (split.size() == 2) {
                actualReadInterval = split[1].toInt(&ok);
                if (ok) {
                    continue;
                }
            }
        }

        // Handle properties.
        else {
            SIGlobalPropertyID propertyID(line.trimmed());
            if (propertyID.isValid() && actualReadInterval) {
                // TODO: Check for duplicates.
                dataLogConfiguration.properties_[actualReadInterval].append(propertyID);
                continue;
            }
        }

        throw std::runtime_error(QString("syntax error on line %1: %2").arg(lineNumber).arg(QString(line)).toStdString());
    }

    dataLogConfiguration.valid_ = true;
    return dataLogConfiguration;
}
