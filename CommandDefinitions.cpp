#include "Commands.h"

std::vector<CommandDefinition> implementedCommands = {
    { { "write", "log", "print" }, [](const Input& input, const CommandDefinitions* commandDefs) -> CommandResult {
        if (input.args.size() < 1 || input.args.size() > 2) return { CommandResult::Type::TooFewArgumentsError, "log takes in 1 or 2 arguments, not " + std::to_string(input.args.size()) };

        if (input.args.size() == 2) {
            if (input.args[1] == "normal") toNormalTextColor();
            else if (input.args[1] == "faded") toFadedTextColor();
            else if (input.args[1] == "error") toErrorTextColor();
            else return { CommandResult::Type::InvalidArgumentTypeError, "if the second argument of log is specified it has to be normal, faded, or error" };
        }

        std::cout << std::endl << input.args[0] << std::endl;

        return {};
    }, "Writes to the console, takes in the message you want to write and a string representing what color the text should be written with, possible values are (normal (default), faded, error)" },

    { { "quit", "exit" }, [](const Input& input, const CommandDefinitions* commandDefs) -> CommandResult {
        if (input.args.size() > 0) return { CommandResult::Type::TooManyArgumentsError, "exit takes 0 arguments" };
        return { CommandResult::Type::ExitProgram };
    }, "Closes the program" },

    { { "info", "help" }, [](const Input& input, const CommandDefinitions* commandDefs) -> CommandResult {
        std::string info = "";

        if (input.args.size() > 0) return { CommandResult::Type::TooManyArgumentsError, "No arguments were expected" };

        for (auto& entry : *(commandDefs->commands)) {
            const std::string seperator = ", ";
            std::string separatedNames = "";

            for (auto name = entry.names.begin(); name != entry.names.end(); name++) separatedNames += seperator + *name;
            separatedNames = separatedNames.substr(seperator.length());

            info += separatedNames + ":\t" + entry.description + "\n\n";
        }

        std::cout << std::endl << "Help" << std::endl << "===========" << std::endl << std::endl << info;

        return {};
    }, "Shows all of the commands and their purposes" },
};

std::vector<CommandDefinition>* getImplementedCommands() {
    return &implementedCommands;
}