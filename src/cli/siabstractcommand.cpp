#include "siabstractcommand.h"

SIAbstractCommand* SIAbstractCommand::command(const QString& name) {
    return commands_[name];
}

SIAbstractCommand::SIAbstractCommand(const QString& name) {
    commands_[name] = this;
}

QMap<QString,SIAbstractCommand*> SIAbstractCommand::commands_;

const QMap<QString, SIAbstractCommand*>& SIAbstractCommand::allCommands() {
    return commands_;
}
