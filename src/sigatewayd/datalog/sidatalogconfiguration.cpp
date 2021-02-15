#include "sidatalogconfiguration.h"

SIDataLogConfiguration SIDataLogConfiguration::parse(QIODevice& device) {
    SIDataLogConfiguration dataLogConfiguration;
    QVector<SIGlobalPropertyID> allIDs;

    // If the device is not open yet, try to open it.
    if (!device.isOpen() && !device.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("could not open datalog configuration");
    }

    // Read  file line per line.
    int lineNumber = 0;
    int actualReadInterval = 0;
    while (!device.atEnd()) {
        lineNumber++;
        auto line = device.readLine().trimmed();

        // Ignore comments and empty lines.
        if (line.startsWith("#") || line.trimmed().isEmpty()) {
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

            if (actualReadInterval == 0) {
                throw std::runtime_error(QString("property without interval declaration on line %1: %2").arg(lineNumber).arg(QString(line)).toStdString());
            }

            if (!propertyID.isValid()) {
                throw std::runtime_error(QString("invalid property id on line %1: %2").arg(lineNumber).arg(QString(line)).toStdString());
            }

            for (const auto& existingID: allIDs) {
                if (propertyID == existingID) {
                    throw std::runtime_error(QString("error on line %1: duplicate property ID %2").arg(lineNumber).arg(propertyID.toString()).toStdString());
                }
                if (existingID.isWildcard() && existingID.matches(propertyID)) {
                    throw std::runtime_error(QString("error on line %1: existing wildcard ID %2 matches new ID %3").arg(lineNumber).arg(existingID.toString())
                                                                                                                   .arg(propertyID.toString()).toStdString());
                }
                if (propertyID.isWildcard() && propertyID.matches(existingID)) {
                    throw std::runtime_error(QString("error on line %1: new wildcard ID %2 matches existing ID %3").arg(lineNumber).arg(propertyID.toString())
                                                                                                                   .arg(existingID.toString()).toStdString());
                }
            }
            allIDs.append(propertyID);

            dataLogConfiguration.properties_[actualReadInterval].append(propertyID);
            continue;
        }

        throw std::runtime_error(QString("syntax error on line %1: %2").arg(lineNumber).arg(QString(line)).toStdString());
    }

    dataLogConfiguration.valid_ = true;
    return dataLogConfiguration;
}
