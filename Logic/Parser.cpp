#include "Parser.h"


Parser::Parser() {
	resetDateTime();
	_fullUserInput = "";
	_userCommand = "";
	_commandData = "";
	_lineOpNumber = 0;
}

Parser::Parser(string userInput) {
	resetDateTime();
	_lineOpNumber = 0;
	_fullUserInput = userInput;
	extractUserCommand(_fullUserInput);
	extractDateAndTime(_commandData);
}

void Parser::resetDateTime() {
	_month = 0;
	_day = 0;
	_hour = -1;
	_minute = 0;
	_duration = 1;
}

string Parser::getUserCommand() {
	return _userCommand;
}

void Parser::setCommandData(string commandData) {
	_commandData = commandData;
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

void Parser::extractUserCommand(string fullString) {
	_commandData = removeSpacePadding(fullString);
	if(_commandData == "") {
		_userCommand = "";
		return;
	}
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

void Parser::extractDateAndTime(string input) {
	resetDateTime();
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
				if(!isValidDate()) {
					_month = 0;
					_day = 0;
					//error message, wrong format
				}
			} else {
				separateHourMinute(demarcateDateTime[0]);
				//must set date to be today
				if(!isValidTime()) {
					_hour = -1;
					_minute = 0;
					//error message, wrong format
				}
			}

			break;
			   }

			   // both date and time, 24hrs
		case 2: {
			size_t dateDelimiterPos= findDateDelimiters(demarcateDateTime[0]);
			int count = 0;
			if(dateDelimiterPos != string::npos) {
				separateDayMonth(demarcateDateTime[count]);
				count++;
				if(!isValidDate()) {
					_month = 0;
					_day = 0;
					//error message, wrong format
				}
			}

			separateHourMinute(demarcateDateTime[count]);
			if(!isValidTime()) {
				_hour = -1;
				_minute = 0;
				//error message, wrong format
			} 
			if(count < 1) {
				if(demarcateDateTime[2] == "m") {
					if(_hour == 12) {
						_hour = 0;
					}
				} else if(demarcateDateTime[2] == "p") {
					_hour += 12;
					if(!isValidTime()) {
						_hour -= 12;
					}
				}
			}

			break;
			   }
			   // both date and time, 12hrs
		case 3: {
			size_t dateDelimiterPos= findDateDelimiters(demarcateDateTime[0]);
			if(dateDelimiterPos != string::npos) {
				separateDayMonth(demarcateDateTime[0]);

				if(!isValidDate()) {
					//error message, wrong format
				}
			}

			separateHourMinute(demarcateDateTime[1]);
			if(!isValidTime()) {
				//error message, wrong format
			} else {
				if(demarcateDateTime[2] == "m") {
					if(_hour == 12) {
						_hour = 0;
					}
				} else if(demarcateDateTime[2] == "p") {
					_hour += 12;
					if(!isValidTime()) {
						_hour -= 12;
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
}

bool Parser::isValidDate() {
	if(_day <= 0 || _month <= 0 || _day > 31 || _month > 12) {
		return false;
	}

	switch (_month) {
	case 2: {
		if(_day > 29) {
			return false;
		}
		break;
		   }
	case 4:
	case 6:
	case 9:
	case 11: {
		if(_day > 30) {
			return false;
		}
		break;
		    }
	default: {
		return true;
		    }
	}
	return true;
}

bool Parser::isValidTime() {
	return (_hour >= 0 && _hour < 24 && _minute >= 0 && _minute < 60);
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
		return "";
	}
}

bool Parser::haveValidLineNumber() {
	if(_commandData == "") {
		return false;
	}
	char *end;
	_lineOpNumber = (int)strtol(_commandData.c_str(), &end, 10);
	return (*end == 0 || _lineOpNumber > 0);

}

int Parser::convertStringToInteger(string numberString) {
	char *end;
	return (int)strtol(numberString.c_str(), &end, 10);
}

Parser::~Parser(void) {
}
