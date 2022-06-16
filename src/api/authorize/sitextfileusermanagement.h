#pragma once
#include "siuserauthorizer.h"
#include <QString>
#include <QMap>
#include <QVector>
#include <memory>

class SITextFileUserManagement: public SIUserAuthorizer {
  public:
    SITextFileUserManagement();
    ~SITextFileUserManagement() override;

    const QString& filename() const;

    void setFilename(const QString& filename);

    bool hasUser(const QString& username) const;

  private:
    SIAccessLevel authorizeUser_(const QString& username, const QString& password) const override;
    bool managementSupported_() const override;
    QMap<QString, SIAccessLevel> listUsers_(bool* status = nullptr) const override;
    bool addUser_(const QString& username, const QString& password, SIAccessLevel accessLevel) override;
    bool changeUserPassword_(const QString& username, const QString& password) override;
    bool changeUserAccessLevel_(const QString& username, SIAccessLevel accessLevel) override;
    bool removeUser_(const QString& username) override;
    struct Private_;
    std::unique_ptr<Private_> private_;
};
