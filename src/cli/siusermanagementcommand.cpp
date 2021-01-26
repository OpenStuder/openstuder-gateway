#include "siabstractcommand.h"
#include <sitextfileusermanagement.h>
#ifdef Q_OS_WINDOWS
#include <Windows.h>
#else
#include <termios.h>
#endif

class SIUserManagementCommand: public SIAbstractCommand {
  public:
    SIUserManagementCommand(): SIAbstractCommand("user") {}

  private:
    int run(QStringList arguments, QTextStream& input, QTextStream& output, const QVariantMap& options) override {

        SITextFileUserManagement userManagement;
        if (options.contains("c")) {
            userManagement.setFilename(options["c"].toString() + "/users.txt");
        }

        if (arguments.count() == 0) {
            usage(output);
            return 1;
        }

        if (arguments[0] == "list") {
            arguments.removeFirst();
            return listUsers(arguments, input, output, userManagement);
        }

        if (arguments[0] == "add") {
            arguments.removeFirst();
            return addUser(arguments, input, output, userManagement);
        }

        if (arguments[0] == "pw") {
            arguments.removeFirst();
            return changeUserPassword(arguments, input, output, userManagement);
        }


        if (arguments[0] == "al") {
            arguments.removeFirst();
            return changeUserAccessLevel(arguments, input, output, userManagement);
        }

        if (arguments[0] == "rm") {
            arguments.removeFirst();
            return removeUser(arguments, input, output, userManagement);
        }

        usage(output);

        return 0;
    }

    int listUsers(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        Q_UNUSED(input)

        if (arguments.count() != 0) {
            usage(output);
            return 1;
        }

        bool ok;

        auto users = userManagement.listUsers(&ok);
        if (!ok) {
            output << "Error: Unable to read user file \"" << userManagement.filename() << "\"." << endl;
            return 1;
        }

        output << "User accounts in \"" << userManagement.filename() << "\":" << endl;
        for (const auto& user: users.keys()) {
            output  << "  " << user << ": " << to_string(users[user]) << endl;
        }
        return 0;
    }

    int addUser(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }

        bool ok;
        auto username = arguments[0];

        if (userManagement.hasUser(username)) {
            output << "Error: User already exists." << endl;
            return 1;
        }

        auto password = askPassword_(username, input, output, &ok);
        if (!ok) {
            return 1;
        }

        auto accessLevel = askAccessLevel_(input, output, &ok);
        if (!ok) {
            return 1;
        }


        if (userManagement.addUser(username, password, accessLevel)) {
            output << "User \"" << username << "\" added with access level \"" << to_string(accessLevel) << "\"." << endl;
            return 0;
        } else {
            output << "Error: User could not be added - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    int changeUserPassword(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }

        bool ok;
        auto username = arguments[0];

        if (!userManagement.hasUser(username)) {
            output << "Error: User does not exists." << endl;
            return 1;
        }

        auto password = askPassword_(username, input, output, &ok);
        if (!ok) {
            return 1;
        }

        if (userManagement.changeUserPassword(username, password)) {
            output << "Changed password for user \"" << username << "\"." << endl;
            return 0;
        } else {
            output << "Error: Could not change user's password - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    int changeUserAccessLevel(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }

        bool ok;
        auto username = arguments[0];

        if (!userManagement.hasUser(username)) {
            output << "Error: User does not exists." << endl;
            return 1;
        }

        auto accessLevel = askAccessLevel_(input, output, &ok);
        if (!ok) {
            return 1;
        }

        if (userManagement.changeUserAccessLevel(username, accessLevel)) {
            output << "Changed password for user to \"" << to_string(accessLevel) << "\"." << endl;
            return 0;
        } else {
            output << "Error: Could not change user's access level - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    int removeUser(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        Q_UNUSED(input)

        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }

        auto username = arguments[0];

        if (!userManagement.hasUser(username)) {
            output << "Error: User does not exists." << endl;
            return 1;
        }

        if (userManagement.removeUser(username)) {
            output << "Removed user \"" << username << "\"." << endl;
            return 0;
        } else {
            output << "Error: Could not remove user - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    void usage(QTextStream& output) override {
        output << "Usage: sigwctl user <command>" << endl << endl;
        output << "commands:" << endl;
        output << "  list ..........: Lists all configured users and their access levels." << endl;
        output << "  add <username> : Adds a new user. Password and access level are queried by the command." << endl;
        output << "  pw <username> .: Change password for user <username>. The password  is queried by the command." << endl;
        output << "  al <username> .: Change access level of user <username>. The access level is queried by the command." << endl;
        output << "  rm <username> .: Removes user <username>" << endl;
    }

    QString askPassword_(const QString& username, QTextStream& input, QTextStream& output, bool* ok = nullptr) {
        QString password, verify;

        if (ok != nullptr) {
            *ok = false;
        }

        enableEcho_(false);

        output << "Enter password for user \"" << username << "\": ";
        output.flush();
        input >> password;
        output << endl << "Repeat password: ";
        output.flush();
        input >> verify;
        output << endl;

        enableEcho_(true);

        if (password != verify) {
            output << "Error: Passwords do not match." << endl;
            return {};
        }

        if (ok != nullptr) {
            *ok = true;
        }
        return password;
    }

    SIAccessLevel askAccessLevel_(QTextStream& input, QTextStream& output, bool* ok = nullptr) {
        int accessLevelChoice;

        output << "Choose access level:" << endl
               << "  0 -> None" << endl
               << "  1 -> Basic" << endl
               << "  2 -> Installer" << endl
               << "  3 -> Expert" << endl
               << "  4 -> Qualified service personnel" << endl
               << "Your choice: ";
        output.flush();
        input >> accessLevelChoice;

        if (ok != nullptr) {
            *ok = true;
        }

        switch (accessLevelChoice) {
            case 0: return SIAccessLevel::None;
            case 1: return SIAccessLevel::Basic;
            case 2: return SIAccessLevel::Installer;
            case 3: return SIAccessLevel::Expert;
            case 4: return SIAccessLevel::QualifiedServicePersonnel;
            default:
                output << "Error: Invalid access level choice." << endl;
                if (ok != nullptr) {
                    *ok = false;
                }
                return SIAccessLevel::None;
        }
    }

    void enableEcho_(bool enable) {

#ifdef Q_OS_WIN32

        HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
        DWORD  mode;

        GetConsoleMode(stdinHandle, &mode);

        if (!enable)
        {
            mode &= ~ENABLE_ECHO_INPUT;
        }
        else
        {
            mode |= ENABLE_ECHO_INPUT;
        }

        SetConsoleMode(stdinHandle, mode);

#else

        struct termios tty {};
        tcgetattr(0, &tty);
        if (!enable) {
            tty.c_lflag &= ~ECHO;
        } else {
            tty.c_lflag |= ECHO;
        }
        tcsetattr(0, TCSANOW, &tty);

#endif

    }
};

static SIUserManagementCommand userManagement;
