//@author A0111951N
#include "Parser.h"
#include "easylogging++.h"

const string Parser::ERROR_NO_LINE_NUMBER = "No line number specified!";
const string Parser::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!";

Parser::Parser() {
	_userCommand = "";
	_item.initializeItem();
}


void Parser::setStringToParse(string userInput) {
	_item.initializeItem();
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

	int lineNum;
	vector<int> numVector;

	char *end;
	lineNum = (int)strtol(_item.event.c_str(), &end, 10);

	while(lineNum > 0) {
		char tempChar = *end;
		int tempInt = lineNum;
		numVector.push_back(lineNum);	
		lineNum = (int)strtol(end + 1, &end, 10);

		if((tempChar == '-') && (lineNum > tempInt)) {
			for(int i = 1; i < (lineNum - tempInt); i++) {
				numVector.push_back(tempInt + i);
			}
		}
	}

	if (numVector.empty() || lineNum < 0) {
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
	return (inputLine.find_last_of("["));
}

// try - catch to be moved to Controller?
void Parser::extractDateAndTime() {
	size_t frontBracketPos = findFrontBracket(_item.event);

	if (frontBracketPos != string::npos) {
		string rawDateTimeChunk = _item.event.substr(frontBracketPos + 1);
		_item.event = removeSpacePadding(_item.event.substr(0, frontBracketPos));
		rawDateTimeChunk = convertStringToLowerCase(rawDateTimeChunk);

		try {
			_splitDateTime.updateItemDateTime(rawDateTimeChunk, _item);
		} catch (const out_of_range& e) {
			e;
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

//@author SHANSHAN
vector <string> Parser::getFragmentedEvent(){
	vector<string> outputVec;

	string wholeEvent = _item.event;
	outputVec.push_back(wholeEvent); 
	size_t spacePos = wholeEvent.find_first_of(" ");

	while(spacePos != string::npos){
		outputVec.push_back(wholeEvent.substr(wholeEvent.find_first_not_of(" ", spacePos+1))); 
		wholeEvent = wholeEvent.substr(wholeEvent.find_first_not_of(" ", spacePos+1));
		spacePos = wholeEvent.find_first_of(" ");
	}

	int startHour = _item.getHour(_item.eventStartTime[0]);
	std::string startHr = std::to_string(startHour);
	string startMin = _item.getMinute(_item.eventStartTime[1]);
	string startTime = startHr  + startMin;

	int endHour = _item.getHour(_item.eventEndTime[0]);
	std::string endHr = std::to_string(endHour);
	string endMin = _item.getMinute(_item.eventEndTime[1]);
	string endTime = endHr + endMin;

	string monthStr = _item.itemDate.getMonthFull(_item.eventDate[1]);
	string weekDay;
	weekDay = _item.itemDate.getWeekDay(_item.eventDate[0], _item.eventDate[1], _item.eventDate[2]);

	//in sequence, the vector contains:
	//weekday, day, integer month, 
	//month, year, start time, end time.
	if(weekDay != ""){
		outputVec.push_back(weekDay);
	}

	if(_item.eventDate[0] != 0){
		std::string tempStr1 = std::to_string(_item.eventDate[0]);
		outputVec.push_back(tempStr1);
	}

	if(_item.eventDate[1] != 0){
		std::string tempStr2 = std::to_string(_item.eventDate[1]);
		outputVec.push_back(tempStr2);
	}

	if(monthStr != ""){
		outputVec.push_back(monthStr);
	}

	if(_item.eventDate[2] != 0){
		std::string tempStr3 = std::to_string(_item.eventDate[2]);
		outputVec.push_back(tempStr3);
	}

	if(startTime != "0"){
		outputVec.push_back(startTime);
	}

	if(endTime != "0"){
		outputVec.push_back(endTime);
	}

	return outputVec;
}

Parser::~Parser(void) {
}
