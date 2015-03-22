#include "Parser.h"
#include "easylogging++.h"

const string Parser::ERROR_NO_LINE_NUMBER = "No line number specified!";
const string Parser::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!";
Parser::Parser() {
	_userCommand = "";
	_lineOpNumber = 0;
	_item.initilizeItem();
}


void Parser::setStringToParse(string userInput) {
	_item.event = userInput;
}

string Parser::getUserCommand() {
	return _userCommand;
}

Item Parser::getItem() {
	return _item;
}

vector<int> Parser::getLineOpNumber() {
	if (_item.event == "") {
		throw std::out_of_range(ERROR_NO_LINE_NUMBER);
	}

	int temp;
	vector<int> numVector;

	char *end;
	temp = (int)strtol(_item.event.c_str(), &end, 10);

	while(temp > 0) {
		numVector.push_back(temp);
		temp = (int)strtol(end + 1, &end, 10);
	}

	if (numVector.empty() || temp < 0) {
		throw std::out_of_range(ERROR_INVALID_LINE_NUMBER);
	}

	return numVector;
}

void Parser::extractUserCommand() {
	_item.event = removeSpacePadding(_item.event);
	if (_item.event == "") {
		_userCommand = "";
		return;
	}
	size_t spacePos = _item.event.find_first_of(" ");
	if (spacePos == string::npos) {
		_userCommand = _item.event;
		_userCommand = convertStringToLowerCase(_userCommand);
		_item.event = "";
	} else {
		_userCommand = _item.event.substr(0, spacePos);
		_userCommand = convertStringToLowerCase(_userCommand);
		_item.event = _item.event.substr(spacePos);
		spacePos = _item.event.find_first_not_of(" ");
		_item.event = _item.event.substr(spacePos);
	}

}

size_t Parser::findFrontBracket(string inputLine) {
	return (inputLine.find_first_of("["));
}

// try - catch to be moved to Controller?
void Parser::extractDateAndTime() {
	size_t frontBracketPos = findFrontBracket(_item.event);

	if (frontBracketPos != string::npos) {
		string rawDateTimeChunk = _item.event.substr(frontBracketPos + 1);
		_item.event = removeSpacePadding(_item.event.substr(0, frontBracketPos));
		convertStringToLowerCase(rawDateTimeChunk);

		try {
			_splitDateTime.updateItemDateTime(rawDateTimeChunk, _item);
		} catch (const out_of_range& e) {

		}
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
