#pragma once

#include <iostream>
#include <windows.h>
#include <set>
#include <sstream>

#include "Commands.h"

// Overriding function declirations in Commands.h
void toNormalTextColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void toFadedTextColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
}

void toErrorTextColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
}

// Global Variables
CommandDefinitions implementedCommandDefinitions = CommandDefinitions(getImplementedCommands());

Input currentInput = {};

// Main functions
void throwError(std::string message) {
    toErrorTextColor();
    std::cout << std::endl << "Error: " << message << std::endl;
}

const bool receiveInput(Input& input) {
    std::string inputStr = "";

    toFadedTextColor();
    std::cout << std::endl << "Command: ";
    toNormalTextColor();
    std::getline(std::cin, inputStr);

    Input::ParseResult parseResult = Input::tryParse(&inputStr, input);

    if (!parseResult.isError) return true;
    else {
        throwError("Invalid input: " + parseResult.description);
        return receiveInput(input);
    }
}

int onInputReceived() {
    toNormalTextColor();

    CommandResult result = {};

    if (implementedCommandDefinitions.tryExecute(currentInput, result)) {
        // processing the result

        if (result.type == CommandResult::Type::ExitProgram) return 0; // because of the way I've set everything up, this is just like returning 0 in main()

        if (result.isError()) throwError(result.message);
    } else throwError("'" + currentInput.commandName + "' not found.");

    receiveInput(currentInput);
    return onInputReceived(); // after receiving new input, return the result of processing it by calling this function
}

void initialize() {
    toFadedTextColor();

    std::cout << "Welcome to my terminal, user" << std::endl << "To start, type 'help' or 'info', then press enter" << std::endl << std::endl;
}

int main() {
    initialize();

    receiveInput(currentInput);
    return onInputReceived();
}