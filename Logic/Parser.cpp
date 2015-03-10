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



void Parser::setDay(int dayData) {
	day = dayData;
}


void Parser::setMonth(int monthData) {
	month = monthData;
}


void Parser::setHour(int hourData) {
	hour = hourData;
}

void Parser::setMinute(int minuteData) {
	minute = minuteData;
}


void Parser::setLineOpNumber(int lineNumber) {
	lineOpNumber = lineNumber;
}


string Parser::getUserCommand() {
	return userCommand;
}


string Parser::getCommandData() {
	return commandData;
}


int Parser::getDay() {
	return day;
}


int Parser::getMonth() {
	return month;
}


int Parser::getHour() {
	return hour;
}

int Parser::getMinute() {
	return minute;
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

size_t Parser::findFrontBracket(string commandData){
	return (commandData.find_first_of("["));
}

size_t Parser::findDateDelimiters(string commandData){
	return (commandData.find_first_of("/._"));
}

void Parser::clearDateTime() {
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
	duration = 1;
}

void Parser::extractDateAndTime() {
	clearDateTime();
	size_t frontBracketPos = findFrontBracket(commandData);

	if(frontBracketPos != string::npos) {
		string rawDateTimeChunk = commandData.substr(frontBracketPos + 1);
		//remove date and time data from commandData
		commandData = commandData.substr(0, frontBracketPos);
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
				
				if(day == 0 || month == 0) {
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
	day = (int)strtol(dayMonth.c_str(), &intEnd, 10);
	month = (int)strtol(intEnd + 1, &intEnd, 10);
}

void Parser::separateHourMinute(string hourMinute) {
	char *intEnd;
	hour = (int)strtol(hourMinute.c_str(), &intEnd, 10);
	minute = (int)strtol(intEnd + 1, &intEnd, 10);
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

int Parser::convertStringToInteger(string numberString) {
	char *end;
	return (int)strtol(numberString.c_str(), &end, 10);
}

Parser::~Parser(void) {
}
