#pragma once
#include "siuserauthorizer.h"
#include <QString>
#include <QMap>
#include <QVector>

class SITextFileUserManagement: public SIUserAuthorizer {
  public:
    SITextFileUserManagement();

    const QString& filename() const {
        return filename_;
    }

    void setFilename(const QString& filename) {
        filename_ = filename;
    }

    QMap<QString, SIAccessLevel> listUsers() const;
    bool addUser(const QString& username, const QString& password, SIAccessLevel accessLevel);
    bool changeUserPassword(const QString& username, const QString& password);
    bool changeUserAccessLevel(const QString& username, SIAccessLevel accessLevel);
    bool removeUser(const QString& username);

  private:
    struct User {
        QString username;
        QString passwordHash;
        SIAccessLevel accessLevel;
    };

    class Users: public QVector<User> {
      public:
        iterator findByUsername(const QString& username);

        inline bool hasByUserName(const QString& username) {
            return findByUsername(username) != end();
        }
    };

    SIAccessLevel authorizeUser_(const QString& username, const QString& password) const override;

    bool load_(Users& users) const;
    bool save_(const Users& users);

    QString filename_;
};
