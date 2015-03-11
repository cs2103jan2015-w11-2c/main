#include "Parser.h"


Parser::Parser(string userInput) {
	_fullUserInput = userInput;
	extractUserCommand();
}


void Parser::setCommand(string command) {
	_userCommand = command;
}


void Parser::setCommandData(string data) {
	_commandData = data;
}



void Parser::setDay(int dayData) {
	_day = dayData;
}


void Parser::setMonth(int monthData) {
	_month = monthData;
}


void Parser::setHour(int hourData) {
	_hour = hourData;
}

void Parser::setMinute(int minuteData) {
	_minute = minuteData;
}


void Parser::setLineOpNumber(int lineNumber) {
	_lineOpNumber = lineNumber;
}


string Parser::getUserCommand() {
	return _userCommand;
}


string Parser::getCommandData() {
	return _commandData;
}


int Parser::getDay() {
	return _day;
}


int Parser::getMonth() {
	return _month;
}


int Parser::getHour() {
	return _hour;
}

int Parser::getMinute() {
	return _minute;
}


int Parser::getLineOpNumber() {
	return _lineOpNumber;
}

void Parser::extractUserCommand() {
	_commandData = removeSpacePadding(_fullUserInput);
	size_t spacePos = _commandData.find_first_of(" ");
	if (spacePos == string::npos) {
		_userCommand = _commandData;
		_commandData = "";
	} else {
		_userCommand = _commandData.substr(0, spacePos);
		_commandData = _commandData.substr(spacePos);
		spacePos = _commandData.find_first_not_of(" ");
		_commandData = _commandData.substr(spacePos);
	}
}

size_t Parser::findFrontBracket(string commandData){
	return (commandData.find_first_of("["));
}

size_t Parser::findDateDelimiters(string commandData){
	return (commandData.find_first_of("/._"));
}

void Parser::clearDateTime() {
	_month = 0;
	_day = 0;
	_hour = 0;
	_minute = 0;
	_duration = 1;
}

void Parser::extractDateAndTime() {
	clearDateTime();
	size_t frontBracketPos = findFrontBracket(_commandData);

	if(frontBracketPos != string::npos) {
		string rawDateTimeChunk = _commandData.substr(frontBracketPos + 1);
		//remove date and time data from commandData
		_commandData = _commandData.substr(0, frontBracketPos);
		istringstream iss (rawDateTimeChunk);
		string demarcateDateTime[3];
		int i = 0;
		while(iss >> demarcateDateTime[i]) {
			i++;
		}

		switch (i) {
		// no date or time
		case 0: {
			//date = today
			break;
			   }

		// only date or only time
		case 1: {
			size_t dateDelimiterPos= findDateDelimiters(demarcateDateTime[0]);
			if(dateDelimiterPos != string::npos) {
				separateDayMonth(demarcateDateTime[0]);
			} else {
				separateHourMinute(demarcateDateTime[0]);
				//must set date to be today
			}

			break;
			   }

		// both date and time, 24hrs
		case 2: {
			size_t dateDelimiterPos= findDateDelimiters(demarcateDateTime[0]);
			if(dateDelimiterPos != string::npos) {
				separateDayMonth(demarcateDateTime[0]);
				
				if(_day == 0 || _month == 0) {
					//error message, wrong format
				}
			}

			separateHourMinute(demarcateDateTime[1]);

			break;
			   }
		// both date and time, 12hrs
		case 3: {

			break;
			   }

		default: {
			break;
			    }
		}
	}
}

void Parser::separateDayMonth(string dayMonth) {
	char *intEnd;
	_day = (int)strtol(dayMonth.c_str(), &intEnd, 10);
	_month = (int)strtol(intEnd + 1, &intEnd, 10);
}

void Parser::separateHourMinute(string hourMinute) {
	char *intEnd;
	_hour = (int)strtol(hourMinute.c_str(), &intEnd, 10);
	_minute = (int)strtol(intEnd + 1, &intEnd, 10);
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
	if(_commandData == "") {
		return false;
	}
	char *end;
	_lineOpNumber = (int)strtol(_commandData.c_str(), &end, 10);
	return (*end == 0);
}

int Parser::convertStringToInteger(string numberString) {
	char *end;
	return (int)strtol(numberString.c_str(), &end, 10);
}

Parser::~Parser(void) {
}
