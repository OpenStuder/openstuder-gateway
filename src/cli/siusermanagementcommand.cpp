#include "siabstractcommand.h"
#include <sitextfileusermanagement.h>

class SIUserManagementCommand: public SIAbstractCommand {
  public:
    SIUserManagementCommand(): SIAbstractCommand("user") {}

  private:
    int run(const QStringList& arguments, QTextStream& input, QTextStream& output) override {
        Q_UNUSED(input)
        Q_UNUSED(output)

        if (arguments.count() == 0) {
            usage(output);
            return 1;
        }

        if (arguments[0] == "list") {
            auto passed = arguments;
            passed.removeFirst();
            return listUsers(passed, input, output);
        }

        if (arguments[0] == "add") {
            auto passed = arguments;
            passed.removeFirst();
            return addUser(passed, input, output);
        }

        return 0;
    }

    int listUsers(const QStringList& arguments, QTextStream& input, QTextStream& output) {
        Q_UNUSED(arguments)
        Q_UNUSED(input)

        SITextFileUserManagement userManagement;
        auto users = userManagement.listUsers();
        for (const auto& user: users.keys()) {
            output << user << ": " << to_string(users[user]) << endl;
        }
        return 0;
    }

    int addUser(const QStringList& arguments, QTextStream& input, QTextStream& output) {
        Q_UNUSED(arguments)
        Q_UNUSED(input)
        Q_UNUSED(output)

        SITextFileUserManagement userManagement;
        return userManagement.addUser("toto", "titi", SIAccessLevel::Basic);
    }

    void usage(QTextStream& output) override {
        output << "Usage: sigwctl user <command>" << endl << endl;
        output << "commands:" << endl;
        output << "  list" << endl;
        output << "  add <username> <accesslevel>" << endl;
    }
};



static SIUserManagementCommand userManagement;
