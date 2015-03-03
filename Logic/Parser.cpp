#include "Parser.h"


Parser::Parser(string userInput) {
	fullUserInput = userInput;
	extractUserCommand();
}

void Parser::setCommand(string command) {
	userCommand = command;
}

void Parser::setCommandData(string data) {
	commandData = data;
}

void Parser::setlineOpNumber(int lineNumber) {
	lineOpNumber = lineNumber;
}

string Parser::getUserCommand() {
	return userCommand;
}

string Parser::getCommandData() {
	return commandData;
}

int Parser::getLineOpNumber() {
	return lineOpNumber;
}

void Parser::extractUserCommand() {
	commandData = removeSpacePadding(fullUserInput);
	size_t spacePos = commandData.find_first_of(" ");
	if (spacePos == string::npos) {
		userCommand = commandData;
		commandData = "";
	} else {
		userCommand = commandData.substr(0, spacePos);
		commandData = commandData.substr(spacePos);
		spacePos = commandData.find_first_not_of(" ");
		commandData = commandData.substr(spacePos);
	}
}

string Parser::removeSpacePadding(string line) {
	size_t end = line.find_last_not_of(" ");
	size_t start = line.find_first_not_of(" ");
	if (start != string::npos) {
		return line.substr(start, (end - start + 1));
	} else if (line == "") {
		return "";
	} else {
		return line.substr(1);
	}
}

bool Parser::getIntegerLineNumber() {
	if(commandData == "") {
		return false;
	}
	char *end;
	lineOpNumber = (int)strtol(commandData.c_str(), &end, 10);
	return (*end == 0);
}

Parser::~Parser(void) {
}
