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

    QMap<QString, SIAccessLevel> listUsers(bool* status = nullptr) const;
    bool hasUser(const QString& username) const;
    bool addUser(const QString& username, const QString& password, SIAccessLevel accessLevel);
    bool changeUserPassword(const QString& username, const QString& password);
    bool changeUserAccessLevel(const QString& username, SIAccessLevel accessLevel);
    bool removeUser(const QString& username);

  private:
    SIAccessLevel authorizeUser_(const QString& username, const QString& password) const override;

    struct Private_;
    std::unique_ptr<Private_> private_;
};
