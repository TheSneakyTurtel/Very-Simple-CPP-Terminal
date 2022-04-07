#pragma once

#include <iostream>
#include <vector>
#include <sstream>

struct Input {
public:
	struct ParseResult {
		bool isError = false;
		std::string description = "";
	};

	static ParseResult tryParse(std::string* str, Input& result) {
		size_t firstSpacePos = str->find(' ');

		if (firstSpacePos == str->npos || firstSpacePos == str->length() - 1) {
			result.commandName = *str;
			result.args = {};
			return {};
		}

		result.commandName = str->substr(0, firstSpacePos);
		result.args = { "" };

		bool inQuotation = false, backSlash = false;
		for (auto& letter : str->substr(firstSpacePos + 1)) {
			size_t lastArgIndex = result.args.size() - 1;

			if (backSlash) {
				if (letter == 'n') result.args[lastArgIndex] += '\n';
				else if (letter == 'r') result.args[lastArgIndex] += '\r';
				else if (letter == 't') result.args[lastArgIndex] += '\t';
				else {
					result.args[lastArgIndex] = "";
					return { true, "There's an invalid character after a backslash (\\)" };
				}

				backSlash = false;
			} else {
				if (letter == '"') inQuotation = !inQuotation;
				else if (letter == ',' && !inQuotation) result.args.push_back("");
				else if (letter == '\\' && inQuotation) backSlash = true;
				else if (inQuotation) result.args[lastArgIndex] += letter;
			}
		}

		if (inQuotation) return { true, "There's an odd number of active quotations (\" symbols with no \\ before them)" };
		return {};
	}

	std::string commandName;
	std::vector<std::string> args;

	Input(std::string _commandName = "", std::vector<std::string> _args = {}) {
		commandName = _commandName;
		args = _args;
	}
};