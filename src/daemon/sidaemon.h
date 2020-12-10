#pragma once
#include <QCoreApplication>
#include <memory>

class SIStorage;
class QSettings;
class SIAbstractPropertyManager;

class SIDaemon: public QCoreApplication {
    Q_OBJECT

  public:
    SIDaemon(int argc, char** args);
    ~SIDaemon() override;

    bool initialize();

  private:
    static QVariantMap filteredChildSettings_(const QSettings& settings, const QStringList& exclude);
    static QStringList filteredChildGroups_(const QSettings& settings, const QStringList& exclude);

    std::unique_ptr<SIStorage> storage_;
    std::unique_ptr<SIAbstractPropertyManager> propertyManager_;
};
