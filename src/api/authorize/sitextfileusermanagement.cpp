#include "sitextfileusermanagement.h"
#include <algorithm>
#include <QCryptographicHash>
#include <QTextStream>
#include <QFile>

SITextFileUserManagement::SITextFileUserManagement() {
    filename_ = OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION "/users.txt";
}

SITextFileUserManagement::Users::iterator SITextFileUserManagement::Users::findByUsername(const QString& username) {
    return std::find_if(begin(), end(), [&username](const User& user) {
        return user.username == username;
    });
}

QMap<QString, SIAccessLevel> SITextFileUserManagement::listUsers(bool* status) const {
    QMap<QString, SIAccessLevel> userListing;
    Users users;
    if (load_(users)) {
        for (const auto& user: users) {
            userListing[user.username] = user.accessLevel;
        }
        if (status != nullptr) {
            *status = true;
        }
    } else {
        if (status != nullptr) {
            *status = false;
        }
    }
    return userListing;
}

bool SITextFileUserManagement::hasUser(const QString& username) const {
    Users users;
    if (!load_(users, false)) return false;
    return users.hasByUserName(username);
}

bool SITextFileUserManagement::addUser(const QString& username, const QString& password, SIAccessLevel accessLevel) {
    Users users;
    if (!load_(users, false)) return false;
    if (users.hasByUserName(username)) return false;
    users.append({username, QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex(), accessLevel});
    return save_(users);
}

bool SITextFileUserManagement::changeUserPassword(const QString& username, const QString& password) {
    Users users;
    if (!load_(users)) return false;
    auto user = users.findByUsername(username);
    if (user == users.end()) return false;
    user->passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex();
    return save_(users);
}

bool SITextFileUserManagement::changeUserAccessLevel(const QString& username, SIAccessLevel accessLevel) {
    Users users;
    if (!load_(users)) return false;
    auto user = users.findByUsername(username);
    if (user == users.end()) return false;
    user->accessLevel = accessLevel;
    return save_(users);
}

bool SITextFileUserManagement::removeUser(const QString& username) {
    Users users;
    if (!load_(users)) return false;
    auto user = users.findByUsername(username);
    if (user == users.end()) return false;
    users.erase(user);
    return save_(users);
}

SIAccessLevel SITextFileUserManagement::authorizeUser_(const QString& username, const QString& password) const {
    Users users;
    if (!load_(users)) return SIAccessLevel::None;
    auto user = users.findByUsername(username);
    if (user == users.end()) return SIAccessLevel::None;
    if (user->passwordHash != QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex())  return SIAccessLevel::None;
    return user->accessLevel;
}

bool SITextFileUserManagement::load_(Users& users, bool checkFileExist) const {
    QFile file(filename_);

    if (!file.exists()) {
        return !checkFileExist;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        auto line = in.readLine();
        auto userData = line.split(":");
        if (userData.count() != 3) return false;
        User user;
        user.username = userData[0];
        user.passwordHash = userData[1];
        if (userData[2] == "None") user.accessLevel = SIAccessLevel::None;
        else if (userData[2] == "Basic") user.accessLevel = SIAccessLevel::Basic;
        else if (userData[2] == "Installer") user.accessLevel = SIAccessLevel::Installer;
        else if (userData[2] == "Expert") user.accessLevel = SIAccessLevel::Expert;
        else if (userData[2] == "QSP") user.accessLevel = SIAccessLevel::QualifiedServicePersonnel;
        else return false;
        users.append(std::move(user));
    }

    return true;
}

bool SITextFileUserManagement::save_(const Users& users) {
    QFile file(filename_);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QTextStream out(&file);
    for (const auto& user: users) {
        out << user.username << ":" << user.passwordHash << ":" << to_string(user.accessLevel) << endl;
    }

    file.close();

    return true;
}
