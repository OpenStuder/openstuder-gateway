#include "sitextfileusermanagement.h"
#include <algorithm>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QTextStream>
#include <QFile>
#include <utility>

using namespace std;

#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define endl Qt::endl
#else
#include <ostream>
#endif

struct SITextFileUserManagement::Private_ {
    struct User {
        QString username;
        QString passwordHash;
        SIAccessLevel accessLevel = SIAccessLevel::None;
    };

    class Users: public QVector<User> {
      public:
        iterator findByUsername(const QString& username);

        inline bool hasByUserName(const QString& username) {
            return findByUsername(username) != end();
        }
    };

    explicit Private_(QString filename): filename(move(filename)) {}

    bool load_(Users& users, bool checkFileExist = false) const;
    bool save_(const Users& users);

    static QString encodePassword_(const QString& password);
    static bool checkPassword_(const QString& password, const QString& encoded);

    QString filename;
};

SITextFileUserManagement::SITextFileUserManagement(): private_(new Private_(OPENSTUDER_GATEWAY_DEFAULT_CONFIG_LOCATION "/users.txt")) {}

SITextFileUserManagement::~SITextFileUserManagement() = default;

const QString& SITextFileUserManagement::filename() const {
    return private_->filename;
}

void SITextFileUserManagement::setFilename(const QString& filename) {
    private_->filename = filename;
}

QMap<QString, SIAccessLevel> SITextFileUserManagement::listUsers(bool* status) const {
    QMap<QString, SIAccessLevel> userListing;
    Private_::Users users;
    if (private_->load_(users)) {
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
    Private_::Users users;
    if (!private_->load_(users, false)) return false;
    return users.hasByUserName(username);
}

bool SITextFileUserManagement::addUser(const QString& username, const QString& password, SIAccessLevel accessLevel) {
    Private_::Users users;
    if (!private_->load_(users, false)) return false;
    if (users.hasByUserName(username)) return false;
    users.append({username, Private_::encodePassword_(password), accessLevel});
    return private_->save_(users);
}

bool SITextFileUserManagement::changeUserPassword(const QString& username, const QString& password) {
    Private_::Users users;
    if (!private_->load_(users)) return false;
    auto user = users.findByUsername(username);
    if (user == users.end()) return false;
    user->passwordHash = Private_::encodePassword_(password);
    return private_->save_(users);
}

bool SITextFileUserManagement::changeUserAccessLevel(const QString& username, SIAccessLevel accessLevel) {
    Private_::Users users;
    if (!private_->load_(users)) return false;
    auto user = users.findByUsername(username);
    if (user == users.end()) return false;
    user->accessLevel = accessLevel;
    return private_->save_(users);
}

bool SITextFileUserManagement::removeUser(const QString& username) {
    Private_::Users users;
    if (!private_->load_(users)) return false;
    auto user = users.findByUsername(username);
    if (user == users.end()) return false;
    users.erase(user);
    return private_->save_(users);
}

SIAccessLevel SITextFileUserManagement::authorizeUser_(const QString& username, const QString& password) const {
    Private_::Users users;
    if (!private_->load_(users)) return SIAccessLevel::None;
    auto user = users.findByUsername(username);
    if (user == users.end()) return SIAccessLevel::None;
    if (!Private_::checkPassword_(password, user->passwordHash)) return SIAccessLevel::None;
    return user->accessLevel;
}

SITextFileUserManagement::Private_::Users::iterator SITextFileUserManagement::Private_::Users::findByUsername(const QString& username) {
    return std::find_if(begin(), end(), [&username](const User& user) {
        return user.username == username;
    });
}

bool SITextFileUserManagement::Private_::load_(Users& users, bool checkFileExist) const {
    QFile file(filename);

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
        users.append(move(user));
    }

    return true;
}

bool SITextFileUserManagement::Private_::save_(const Users& users) {
    QFile file(filename);

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

QString SITextFileUserManagement::Private_::encodePassword_(const QString& password) {
    auto salt = QByteArray(16, 0);
    QRandomGenerator::system()->generate(salt.begin(), salt.end());
    return (salt + QCryptographicHash::hash(password.toUtf8() + salt, QCryptographicHash::Sha512)).toBase64();
}

bool SITextFileUserManagement::Private_::checkPassword_(const QString& password, const QString& encoded) {
    auto saltAndHash = QByteArray::fromBase64(encoded.toUtf8());
    return QCryptographicHash::hash(password.toUtf8() + saltAndHash.left(16), QCryptographicHash::Sha512) == saltAndHash.mid(16);
}
