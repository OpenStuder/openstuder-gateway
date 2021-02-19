#include "siabstractcommand.h"
#include <sitextfileusermanagement.h>
#ifdef Q_OS_WINDOWS
#include <Windows.h>
#else
#include <termios.h>
#endif

using namespace std;

class SIUserManagementCommand: public SIAbstractCommand {
  public:
    SIUserManagementCommand(): SIAbstractCommand("user") {}

  private:
    int run(QStringList arguments, QTextStream& input, QTextStream& output, const QVariantMap& options) override {

        // Create the text file user management object and configure the file location.
        SITextFileUserManagement userManagement;
        if (options.contains("c")) {
            userManagement.setFilename(options["c"].toString() + "/users.txt");
        }

        // If no subcommand is provided, print usage and quit.
        if (arguments.count() == 0) {
            usage(output);
            return 1;
        }

        // Remove command itself from the argument list.
        auto subcommand = arguments.first();
        arguments.removeFirst();

        // Call requested subcommand if it exists.
        if (subcommand == "list") {
            return listUsers_(arguments, input, output, userManagement);
        }

        if (subcommand == "add") {
            return addUser_(arguments, input, output, userManagement);
        }

        if (subcommand == "pw") {
            return changeUserPassword_(arguments, input, output, userManagement);
        }

        if (subcommand == "al") {
            return changeUserAccessLevel_(arguments, input, output, userManagement);
        }

        if (subcommand == "rm") {
            return removeUser_(arguments, input, output, userManagement);
        }

        // The requested subcommand does not exist, print usage and quit.
        usage(output);
        return 1;
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

    int listUsers_(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        Q_UNUSED(input)

        // The subcommand takes no arguments.
        if (arguments.count() != 0) {
            usage(output);
            return 1;
        }

        // Read users from file and fail if they could not be read.
        bool ok;
        auto users = userManagement.listUsers(&ok);
        if (!ok) {
            output << "Error: Unable to read user file \"" << userManagement.filename() << "\"." << endl;
            return 1;
        }

        // List all user accounts with their access level.
        output << "User accounts in \"" << userManagement.filename() << "\":" << endl;
        for (const auto& user: users.keys()) {
            output  << "  " << user << ": " << to_string(users[user]) << endl;
        }

        // Return success.
        return 0;
    }

    int addUser_(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {

        // The subcommand takes one argument, the username.
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }
        auto username = arguments[0];

        // If there is already a user with that username present, fail.
        if (userManagement.hasUser(username)) {
            output << "Error: User already exists." << endl;
            return 1;
        }

        // Ask the password for the new user on the console.
        auto password = askPassword_(username, input, output);
        if (password.isEmpty()) {
            return 1;
        }

        // Ask interactively the access level for the new user.
        auto accessLevel = askAccessLevel_(input, output);
        if (static_cast<unsigned int>(accessLevel) == numeric_limits<unsigned int>::max()) {
            return 1;
        }

        // Try to add the user and report the status of the operation to the console.
        if (userManagement.addUser(username, password, accessLevel)) {
            output << "User \"" << username << "\" added with access level \"" << to_string(accessLevel) << "\"." << endl;
            return 0;
        } else {
            output << "Error: User could not be added - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    int changeUserPassword_(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {

        // The subcommand takes one argument, the username.
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }
        auto username = arguments[0];

        // Ensure that the user exists.
        if (!userManagement.hasUser(username)) {
            output << "Error: User does not exists." << endl;
            return 1;
        }

        // Ask the new password for new user on the console.
        auto password = askPassword_(username, input, output);
        if (password.isEmpty()) {
            return 1;
        }

        // Try to change the password of the user and report the status to the console.
        if (userManagement.changeUserPassword(username, password)) {
            output << "Changed password for user \"" << username << "\"." << endl;
            return 0;
        } else {
            output << "Error: Could not change user's password - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    int changeUserAccessLevel_(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {

        // The subcommand takes one argument, the username.
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }
        auto username = arguments[0];

        // Ensure that the user exists.
        if (!userManagement.hasUser(username)) {
            output << "Error: User does not exists." << endl;
            return 1;
        }

        // Ask interactively the new access level for the user.
        auto accessLevel = askAccessLevel_(input, output);
        if (static_cast<unsigned int>(accessLevel) == numeric_limits<unsigned int>::max()) {
            return 1;
        }

        // Try to change the user's access level and report the status to the console.
        if (userManagement.changeUserAccessLevel(username, accessLevel)) {
            output << "Changed password for user to \"" << to_string(accessLevel) << "\"." << endl;
            return 0;
        } else {
            output << "Error: Could not change user's access level - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    int removeUser_(const QStringList& arguments, QTextStream& input, QTextStream& output, SITextFileUserManagement& userManagement) {
        Q_UNUSED(input)

        // The subcommand takes one argument, the username.
        if (arguments.count() != 1) {
            usage(output);
            return 1;
        }
        auto username = arguments[0];

        // Ensure that the user exists.
        if (!userManagement.hasUser(username)) {
            output << "Error: User does not exists." << endl;
            return 1;
        }

        // Try to remove the user and report the status to the console.
        if (userManagement.removeUser(username)) {
            output << "Removed user \"" << username << "\"." << endl;
            return 0;
        } else {
            output << "Error: Could not remove user - file might not be writable by the current user, try using sudo." << endl;
            return 1;
        }
    }

    static QString askPassword_(const QString& username, QTextStream& input, QTextStream& output) {

        // Disable echo on console to avoid that the passwords are visible.
        enableEcho_(false);

        // Ask for the password.
        output << "Enter password for user \"" << username << "\": ";
        output.flush();
        QString password;
        input >> password;

        // The user has to repeat the password to verify that it is correct.
        output << endl << "Repeat password: ";
        output.flush();
        QString verify;
        input >> verify;
        output << endl;

        // Enable echo on console.
        enableEcho_(true);

        // Fail if the two passwords do not match.
        if (password != verify) {
            output << "Error: Passwords do not match." << endl;
            return {};
        }

        // Return the password.
        return password;
    }

    static SIAccessLevel askAccessLevel_(QTextStream& input, QTextStream& output) {

        // Show possible access levels and ask for the index.
        output << "Choose access level:" << endl
               << "  0 -> None" << endl
               << "  1 -> Basic" << endl
               << "  2 -> Installer" << endl
               << "  3 -> Expert" << endl
               << "  4 -> Qualified service personnel" << endl
               << "Your choice: ";
        output.flush();
        int accessLevelChoice = 0;
        input >> accessLevelChoice;

        // Convert the index to the respective access level and return and invalid value if a wrong index was choosen.
        switch (accessLevelChoice) {
            case 0: return SIAccessLevel::None;
            case 1: return SIAccessLevel::Basic;
            case 2: return SIAccessLevel::Installer;
            case 3: return SIAccessLevel::Expert;
            case 4: return SIAccessLevel::QualifiedServicePersonnel;
            default:
                output << "Error: Invalid access level choice." << endl;
                return static_cast<SIAccessLevel>(numeric_limits<unsigned int>::max());
        }
    }

    static void enableEcho_(bool enable) {

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

static SIUserManagementCommand userManagement; // NOLINT(cert-err58-cpp)
