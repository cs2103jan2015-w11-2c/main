#include "Parser.h"
#include "easylogging++.h"


Parser::Parser() {
	_fullUserInput = "";
	_userCommand = "";
	_event = "";
	_lineOpNumber = 0;
}

Parser::Parser(string userInput) {
	_lineOpNumber = 0;
	_fullUserInput = userInput;
	
	extractDateAndTime(userInput);
	extractUserCommand(_event);

}

string Parser::getUserCommand() {
	return _userCommand;
}

string Parser::getEvent() {
	return _event;
}

Item Parser::getItem() {
	return _item;
}

int Parser::getLineOpNumber() {
	if (_event == "") {
		throw std::out_of_range("No line number");
	}

	char *end;
	_lineOpNumber = (int)strtol(_event.c_str(), &end, 10);

	if (*end != 0 || _lineOpNumber <= 0) {
		throw std::out_of_range("Invalid line number");
	}

	return _lineOpNumber;
}

void Parser::extractUserCommand(string fullString) {
	_event = removeSpacePadding(fullString);
	if (_event == "") {
		_userCommand = "";
		return;
	}
	size_t spacePos = _event.find_first_of(" ");
	if (spacePos == string::npos) {
		_userCommand = _event;
		_userCommand = convertStringToLowerCase(_userCommand);
		_event = "";
	} else {
		_userCommand = _event.substr(0, spacePos);
		_userCommand = convertStringToLowerCase(_userCommand);
		_event = _event.substr(spacePos);
		spacePos = _event.find_first_not_of(" ");
		_event = _event.substr(spacePos);
		_item.event = _event;
	}

}

size_t Parser::findFrontBracket(string inputLine) {
	return (inputLine.find_first_of("["));
}

void Parser::extractDateAndTime(string input) {
	size_t frontBracketPos = findFrontBracket(input);

	if (frontBracketPos != string::npos) {
		string rawDateTimeChunk = input.substr(frontBracketPos + 1);
		_event = input.substr(0, frontBracketPos);
		convertStringToLowerCase(rawDateTimeChunk);
		_splitDateTime.updateItemDateTime(rawDateTimeChunk, _item);
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
		return "";
	}
}


string Parser::convertStringToLowerCase(string inputString) {
	transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower);
	return inputString;
}

Parser::~Parser(void) {
}
