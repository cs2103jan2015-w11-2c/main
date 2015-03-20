
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
	extractDateAndTime(_event);
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
	size_t frontBracketPos = findFrontBracket(_event);

	if (frontBracketPos != string::npos) {
		string rawDateTimeChunk = _event.substr(frontBracketPos + 1);
		_event = _event.substr(0, frontBracketPos);
<<<<<<< HEAD
		istringstream iss (rawDateTimeChunk);
		string demarcateDateTime[3];
		int i = 0;
		while(iss >> demarcateDateTime[i]) {
			i++;
		}

		switch (i) {
			// no date or time
		case 0: {
			//date = _today
			break;
			   }

			   // only date or only time
		case 1: {
			size_t dateDelimiterPos= findDateDelimiters(demarcateDateTime[0]);
			if(dateDelimiterPos != string::npos) {
				separateDayMonth(demarcateDateTime[0]);
				if(!dateTime.isValidDate(_day, _month, _year)) {
					_month = 0;
					_day = 0;
					//error message, wrong format
				}
			} else {
				separateHourMinute(demarcateDateTime[0]);
				//must set date to be _today
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
				if(!dateTime.isValidDate(_day, _month, _year)) {
					_month = 0;
					_day = 0;
					//error message, wrong format
				}
			}
		    //assert(!dateTime.isValidDate(_day, _month, _year));
			//if assert fails, system crashes.
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

				if(!dateTime.isValidDate(_day, _month, _year)) {
					//error message, wrong format
				}
			}   
			  //assert(!dateTime.isValidDate(_day, _month, _year));
			//if assert fails, system crashes.
			separateHourMinute(demarcateDateTime[1]);
			if(!isValidTime()) {
				//error message, wrong format
			} else {
				//assert(!dateTime.isValidTime)
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

bool Parser::isValidTime() {
	return (_hour >= 0 && _hour < 24 && _minute >= 0 && _minute < 60);
	//assert(hour>=0);
	//assert(hour<24);
	//assert(minute>=0);
	//assert(minute<60)
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



=======
		convertStringToLowerCase(rawDateTimeChunk);
		_splitDateTime.updateItemDateTime(rawDateTimeChunk, _item);
	}
}

>>>>>>> fb54dcf9e44239230d8d133dbfc364a9dfb84d8d
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
