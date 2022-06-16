#pragma once
#include <siaccesslevel.h>
#include <QString>
#include <QMap>

class SIUserAuthorizer {
  public:
    SIUserAuthorizer() = default;
    SIUserAuthorizer(const SIUserAuthorizer&) = delete;
    SIUserAuthorizer& operator =(const SIUserAuthorizer&) = delete;
    virtual ~SIUserAuthorizer() = default;

    inline SIAccessLevel authorizeUser(const QString& username, const QString& password) const {
        return authorizeUser_(username, password);
    }

    inline bool managementSupported() const  {
        return managementSupported_();
    }
    inline QMap<QString, SIAccessLevel> listUsers(bool* status = nullptr) const {
        return listUsers_(status);
    }
    inline bool addUser(const QString& username, const QString& password, SIAccessLevel accessLevel) {
        return addUser_(username, password, accessLevel);
    }
    inline bool changeUserPassword(const QString& username, const QString& password) {
        return changeUserPassword_(username, password);
    }
    inline bool changeUserAccessLevel(const QString& username, SIAccessLevel accessLevel) {
        return changeUserAccessLevel_(username, accessLevel);
    }
    inline bool removeUser(const QString& username) {
        return removeUser_(username);
    }

  private:
    virtual SIAccessLevel authorizeUser_(const QString& username, const QString& password) const = 0;

    virtual bool managementSupported_() const { return false; }
    virtual QMap<QString, SIAccessLevel> listUsers_(bool* status = nullptr) const {
        if (status) *status = false;
        return {};
    }
    virtual bool addUser_(const QString& username, const QString& password, SIAccessLevel accessLevel) {
        Q_UNUSED(username);
        Q_UNUSED(password);
        Q_UNUSED(accessLevel);
        return false;
    }
    virtual bool changeUserPassword_(const QString& username, const QString& password) {
        Q_UNUSED(username);
        Q_UNUSED(password);
        return false;
    }
    virtual bool changeUserAccessLevel_(const QString& username, SIAccessLevel accessLevel) {
        Q_UNUSED(username);
        Q_UNUSED(accessLevel);
        return false;
    }
    virtual bool removeUser_(const QString& username) {
        Q_UNUSED(username);
        return false;
    }
};
