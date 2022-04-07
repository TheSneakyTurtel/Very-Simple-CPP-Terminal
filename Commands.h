#pragma once

#include <iostream>
#include <set>
#include <vector>

#include "Input.h"

void toNormalTextColor();
void toFadedTextColor();
void toErrorTextColor();

struct CommandDefinitions;
struct CommandResult;
struct CommandDefinition;

struct CommandResult {
    enum class Type : short {
        Null = 0,

        ExitProgram = 1,

        InvalidArgumentTypeError = -1,
        TooFewArgumentsError = -2,
        TooManyArgumentsError = -3,
    };

    CommandResult::Type type;
    std::string message;

    CommandResult(CommandResult::Type _type = CommandResult::Type::Null, std::string _message = "") {
        type = _type;
        message = _message;
    }

    bool isError() const {
        return (short)type < 0;
    }
};

struct CommandDefinition {
    std::set<std::string> names;
    CommandResult(*action)(const Input&, const CommandDefinitions*);
    std::string description;

    CommandDefinition operator=(const CommandDefinition& other) {
        names = other.names;
        action = other.action;
        description = other.description;
        return other;
    }
};

struct CommandDefinitions {
public:
    std::vector<CommandDefinition>* commands;

    CommandDefinitions(std::vector<CommandDefinition>* _commands) {
        commands = _commands;
    }

    bool tryExecute(const Input& input, CommandResult& results) {
        for (auto& entry : *commands) {
            // if one of the command's names matches the gien input's command name then we've found the command we're looking for
            if (entry.names.count(input.commandName)) {
                results = entry.action(input, this); // execute the command, and set the results argument to the results
                return true;
            }
        }

        return false;
    }
};

std::vector<CommandDefinition>* getImplementedCommands();