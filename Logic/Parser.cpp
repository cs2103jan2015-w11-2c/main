#include "Parser.h"
#include "easylogging++.h"


Parser::Parser() {
	resetDateTime();
	_fullUserInput = "";
	_userCommand = "";
	_event = "";
	_lineOpNumber = 0;
}

Parser::Parser(string userInput) {
	resetDateTime();
	_lineOpNumber = 0;
	_fullUserInput = userInput;
	extractUserCommand(_fullUserInput);
	extractDateAndTime(_event);
}

void Parser::resetDateTime() {
	_item.eventDate[0] = 0;
	_item.eventDate[1] = 0;
	_item.eventDate[2] = _dateTime.getCurrentYear();
	_item.eventStartTime[0] = -1;
	_item.eventStartTime[1] = 0;
	_item.eventEndTime[0] = -1;
	_item.eventEndTime[1] = 0;
	_item.colour = 0;
	_item.bold = false;
}

void Parser::setDate(int day, int month, int year) {
	_item.eventDate[0] = day;
	_item.eventDate[1] = month;
	_item.eventDate[2] = year;
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
	}

}

size_t Parser::findFrontBracket(string inputLine) {
	return (inputLine.find_first_of("["));
}

size_t Parser::findDateDelimiters(string inputLine) {
	return (inputLine.find_first_of("/._"));
}

void Parser::extractDateAndTime(string input) {
	resetDateTime();
	size_t frontBracketPos = findFrontBracket(_event);

	if (frontBracketPos != string::npos) {
		string rawDateTimeChunk = _event.substr(frontBracketPos + 1);
		_event = _event.substr(0, frontBracketPos);
		splitDateTime(rawDateTimeChunk);
	}
}

void Parser::splitDateTime(string input) {
	istringstream iss(convertStringToLowerCase(input));
	string demarcateDateTime[7];
	int i = 0;
	while (iss >> demarcateDateTime[i]) {
		i++;
	}

	switch (i) {
		// no date or time, set to today
	case 0: {
		setDate(_dateTime.getCurrentDay(), _dateTime.getCurrentMonth(), _dateTime.getCurrentYear());
		break;
	}

			// only date or only time
	case 1: {
		handleOneDateInput(demarcateDateTime);
		break;
	}

			// both date and time, 24hrs
	case 2: {
		handleTwoDateInput(demarcateDateTime);
		break;
	}
			// both date and time, 12hrs
	case 3: {
		size_t dateDelimiterPos = findDateDelimiters(demarcateDateTime[0]);
		if (dateDelimiterPos != string::npos) {
			separateDayMonthYear(demarcateDateTime[0]);

			if (!_dateTime.isValidDate(_item.eventDate[0], _item.eventDate[1], _item.eventDate[2])) {
				//error message, wrong format
			}
		}

		separateHourMinute(demarcateDateTime[1], _item.eventStartTime[0], _item.eventStartTime[1]);
		if (!_dateTime.isValidDate(_item.eventDate[0], _item.eventDate[1], _item.eventDate[2])) {
			//error message, wrong format
		} else {
			if (demarcateDateTime[2] == "m") {
				if (_item.eventStartTime[0] == 12) {
					_item.eventStartTime[0] = 0;
				}
			} else if (demarcateDateTime[2] == "p") {
				_item.eventStartTime[0] += 12;
				if (!isValidTime(_item.eventStartTime[0], _item.eventStartTime[1])) {
					_item.eventStartTime[0] -= 12;
				}
			}
		}
		break;
	}

	default: {
		break;
	}
	}
}

void Parser::handleOneDateInput(string inputArray[]) {
	size_t dateDelimiterPos = findDateDelimiters(inputArray[0]);
	if (dateDelimiterPos != string::npos) {
		separateDayMonthYear(inputArray[0]);
		try {
			verifyItemDate();
		} catch (const out_of_range& e) {
			LOG(ERROR) << "handleOneDateInput throws: " << e.what();
			clog << e.what();
		}
	}
	// else if(WEEKDAY INPUT (E.G. FRIDAY))
	else {
		separateHourMinute(inputArray[0], _item.eventStartTime[0], _item.eventStartTime[1]);
		verifyItemTime(_item.eventStartTime[0], _item.eventStartTime[1]);
		try {
			verifyItemDate();
		} catch (const out_of_range& e) {
			LOG(ERROR) << "handleOneDateInput throws: " << e.what();
			clog << e.what();
		}
	}
}

void Parser::handleTwoDateInput(string inputArray[]) {
	size_t dateDelimiterPos = findDateDelimiters(inputArray[0]);
	int count = 0;
	if (dateDelimiterPos != string::npos) {
		separateDayMonthYear(inputArray[count]);
		verifyItemDate();
		// catch error message
		count++;
	} else if (inputArray[0] == "next") {
	} // else if(inputArray[0] == MAP)
	else {
		separateHourMinute(inputArray[count], _item.eventStartTime[0], _item.eventStartTime[1]);
		verifyItemTime(_item.eventStartTime[0], _item.eventStartTime[1]);
		// catch error message
	}
	if (count < 1) {
		if (inputArray[2] == "m") {
			if (_item.eventStartTime[0] == 12) {
				_item.eventStartTime[0] = 0;
			}
		} else if (inputArray[2] == "p" || inputArray[2] == "pm") {
			_item.eventStartTime[0] += 12;
			if (!isValidTime(_item.eventStartTime[0], _item.eventStartTime[1])) {
				_item.eventStartTime[0] -= 12;
			}
		}
	}

}


bool Parser::isValidTime(int hour, int minute) {
	return (hour >= 0 && hour < 24 && minute >= 0 && minute < 60);
}

void Parser::separateDayMonthYear(string dayMonth) {
	char *intEnd;
	_item.eventDate[0] = (int)strtol(dayMonth.c_str(), &intEnd, 10);
	_item.eventDate[1] = (int)strtol(intEnd + 1, &intEnd, 10);
	_item.eventDate[2] = (int)strtol(intEnd + 1, &intEnd, 10);
}

void Parser::separateHourMinute(string hourMinute, int& hour, int& minute) {
	char *intEnd;
	hour = (int)strtol(hourMinute.c_str(), &intEnd, 10);
	minute = (int)strtol(intEnd + 1, &intEnd, 10);
}

void Parser::verifyItemDate() {
	if (_item.eventDate[2] == 0) {
		_item.eventDate[2] = _dateTime.getCurrentYear();
	} else if (_item.eventDate[2] < 2000) {
		_item.eventDate[2] += 2000;
	}

	if (!_dateTime.isValidDate(_item.eventDate[0], _item.eventDate[1], _item.eventDate[2])) {
		_item.eventDate[0] = 0;
		_item.eventDate[1] = 0;
		throw std::out_of_range("Invalid date input!");
	}
}

void Parser::verifyItemTime(int& hour, int& minute) {
	if (!isValidTime(_item.eventStartTime[0], _item.eventStartTime[1])) {
		_item.eventStartTime[0] = -1;
		_item.eventStartTime[1] = 0;
		throw std::out_of_range("Invalid time input!");
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

int Parser::convertStringToInteger(string numberString) {
	char *end;
	return (int)strtol(numberString.c_str(), &end, 10);
}

string Parser::convertStringToLowerCase(string inputString) {
	transform(inputString.begin(), inputString.end(), inputString.begin(), ::tolower);
	return inputString;
}

Parser::~Parser(void) {
}
