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


void Parser::setEvent(string eventData) {
	eventToBeDone = eventData;
}


void Parser::setDay(string dayData) {
	 day = dayData;
}


void Parser::setMonth(string monthData){
	 month = monthData;
}


void Parser::setTime(string timeData){
     time = timeData;
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


string Parser::getEvent() {
	return eventToBeDone;
}


string Parser::getDay() {
	return day;
}


string Parser::getMonth() {
	return month;
}


string Parser::getTime() {
	return time;
}


int Parser::getLineOpNumber() {
	return lineOpNumber;
}

/*
void Parser::extractAllData() {
	commandData = removeSpacePadding(fullUserInput);
	size_t spacePos = commandData.find_first_of(" ");
	if (spacePos == string::npos) {
		userCommand = commandData
		eventToBeDone = "";
	} else {
		userCommand = commandData.substr(0, spacePos);
		commandData = commandData.substr(spacePos);
		spacePos = commandData.find_first_not_of(" ");
		commandData = commandData.substr(spacePos);
		
		spacePos = commandData.find_first_of(" ");
		eventToBeDone=commandData.substr(0,spacePos);
		eventToBeDone=eventTobeDone.substr(spacePos);
		spacePos = eventTobeDone.find_first_not_of(" ");
		timeOfEvent=eventTobeDone.substr(spacePos);
		extractDateAndTime(timeOfEvent);
	}
}
*/

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
/*
void extractDateAndTime(string eventTime){
        size_t spacePos = timeOfEvent.find_first_of(" ");
	    
		day = timeOfEvent.substr(0,spacePos);
		timeOfEvent = timeOfEvent.substr(spacePos);
		spacePos = eventTime.find_first_not_of(" ");
		timeOfEvent = timeOfEvent.substr(spacePos)
        
		spacePos = timeOfEvent.find_first_of(" ");
		month = timeOfEvent.substr(0,spacePos);
		timeOfEvent = timeOfEvent.substr(spacePos);
		spacePos = timeOfEvent.find_first_not_of(" ");
		time = timeOfEvent.substr(spacePos);
}
*/

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
