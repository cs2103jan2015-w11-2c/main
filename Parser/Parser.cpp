//@author A0111951N
#include "Parser.h"
#include "..\EasyLoggingpp\easylogging++.h"

const string Parser::ERROR_NO_LINE_NUMBER = "No line number specified!";
const string Parser::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!";
const string Parser::STRING_FLOATING = "floating";

Parser::Parser() {
	_userCommand = "";
	_item.initializeItem();
	_isDeadline = false;
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

	LOG(INFO) << "Line number extraction: " << lineNum;

	while(lineNum > 0) {
		char tempChar = *end;
		int tempInt = lineNum;
		numVector.push_back(lineNum);	
		lineNum = (int)strtol(end + 1, &end, 10);
		LOG(INFO) << "Line number extraction: " << lineNum;
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

bool Parser::_isDeadlineEvent() {
	return _isDeadline;
}

size_t Parser::findDateKeyWord(string inputLine, string delimiter) {
	string line = convertStringToLowerCase(inputLine);
	size_t dateStart = line.rfind(delimiter);
	string temp;
	if(dateStart != string::npos) {
		if(line[(dateStart - 1)] == 'm') { //if mon or monday
			string temp = line.substr(0, dateStart);
			dateStart = temp.rfind(delimiter);
		}
		if((dateStart != string::npos) && isCorrectDateDelimiter(line, dateStart)) {
			return dateStart;
		}
	}

	return string::npos;
}

bool Parser::isCorrectDateDelimiter(string inputLine, size_t index) {
	bool isDate = true;
	string temp = inputLine.substr(index);
	size_t spacePos = temp.find_first_of(' ');
	if(spacePos == string::npos) {
		return false;
	}
	temp = temp.substr(spacePos + 1);

	istringstream iss(temp);
	string word;
	while((iss >> word) && isDate) {
		if(!_dateTimeParse.isDateKeyword(word)) {
			isDate = false;
		}
	}
	return isDate;
}


void Parser::extractDateAndTime() {
	size_t delimiterIndex = findDateKeyWord(_item.event, DATE_START_DEADLINE);
	_isDeadline = (delimiterIndex == string::npos) ? false : true;

	if(delimiterIndex == string::npos) {
		delimiterIndex = findDateKeyWord(_item.event, DATE_START_1);
	}

	if(delimiterIndex == string::npos) {
		delimiterIndex = findDateKeyWord(_item.event, DATE_START_2);
	}

	if (delimiterIndex != string::npos) {
		string rawDateTimeChunk = _item.event.substr(delimiterIndex);
		_item.event = removeSpacePadding(_item.event.substr(0, delimiterIndex));
		rawDateTimeChunk = convertStringToLowerCase(rawDateTimeChunk);

		try {
			_dateTimeParse.updateItemDateTime(rawDateTimeChunk, _item, _isDeadline);
		} catch (const out_of_range& e) {
			cout << e.what();
		}
		assertItemValues();

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

void Parser::assertItemValues() {
	assert(_item.eventDate[0] >= 0);
	assert(_item.eventDate[1] >= 0);
	assert(_item.eventDate[2] >= 0);
	assert(_item.eventEndDate[0] >= 0);
	assert(_item.eventEndDate[1] >= 0);
	assert(_item.eventEndDate[2] >= 0);
	assert(_item.eventStartTime[0] >= 0);
	assert(_item.eventStartTime[1] >= 0);
	assert(_item.eventEndTime[0] >= 0);
	assert(_item.eventEndTime[1] >= 0);
}


//@author A0114613U
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
	string startHr = std::to_string(startHour);
	string startMin = _item.getMinute(_item.eventStartTime[1]);
	string startTime = startHr  + startMin;

	int endHour = _item.getHour(_item.eventEndTime[0]);
	string endHr = std::to_string(endHour);
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

	//assert(_item.eventDate[0] >= 0);
	if(_item.eventDate[0] != 0){
		std::string tempStr1 = std::to_string(_item.eventDate[0]);
		outputVec.push_back(tempStr1);
	}

	//assert(_item.eventDate[1] >= 0);
	if(_item.eventDate[1] != 0){
		std::string tempStr2 = std::to_string(_item.eventDate[1]);
		outputVec.push_back(tempStr2);
	}

	if(monthStr != ""){
		outputVec.push_back(monthStr);
	}

	//assert(_item.eventDate[2] >= 0);
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

bool Parser::checkIsFloating(const Item item) {
	for (int i = 0; i < 3; i++) {
		if (item.eventDate[i] != 0) {
			return false;
		}
	}
	for (int i = 0; i < 3; i++) {
		if (item.eventEndDate[i] != 0) {
			return false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (item.eventStartTime[i] != 0) {
			return false;
		}
	}
	for (int i = 0; i < 2; i++) {
		if (item.eventEndTime[i] != 0) {
			return false;
		}
	}
	return true;
}

void Parser::clearStartAndEndDate(Item &item) {
	for (int i = 0; i < 3; i++) {
		item.eventDate[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		item.eventEndDate[i] = 0;
	}
}

bool Parser::checkIsValidDate(const string input) {
	int day = 0;
	int mon = 0;
	int year = 0;

	DateTime dateTime;
	string temp = input;
	size_t pos = temp.find_first_of("/");
	if (pos == string::npos) {
		return false;
	}
	day = stoi(temp.substr(0, pos + 1), NULL, 10);

	if (day == 0) {
		return false;
	}

	temp = temp.substr(pos + 1);

	pos = temp.find_first_of("/");
	if (pos == string::npos) {
		year = dateTime.getCurrentYear();
	} else {
		mon = stoi(temp.substr(0, pos + 1), NULL, 10);
		if (mon == 0) {
			return false;
		}

		temp = temp.substr(pos + 1);
		
		year = stoi(temp, NULL, 10);
		if (year == 0) {
			return false;
		}
	}
	return dateTime.isValidDate(day, mon, year);
}

bool Parser::checkIsDeadline(const string input) {
	istringstream iss(input);
	int i = 0;
	string temp = "";

	while (iss >> temp) {
		if (temp != "") {
			i++;
		}
	}

	if (i == 1) {
		return true;
	}
	return false;
}

void Parser::extractSearchQuery(Item &item) {
	Item temp = item;

	//if(checkIsFloating(temp)) {
		DateTimeParser dateTimeParser;

		string itemEvent = temp.event;
		if (itemEvent != STRING_FLOATING) {
			itemEvent = convertStringToLowerCase(itemEvent);
			string tempEvent = "from " + item.event.substr(item.event.find_first_not_of("\t\n"));
			if (isCorrectDateDelimiter(tempEvent, 0) || checkIsValidDate(item.event)){
				dateTimeParser.updateItemDateTime(itemEvent, temp, checkIsDeadline(item.event));
			}

			if (/*!dateTimeParser.getIsDateUpdatedFromFloat() &&*/
				(dateTimeParser.getUpdateDateFlag() || dateTimeParser.getUpdateTimeFlag())) {
					temp.event = "";
			}
			//if (!dateTimeParser.getUpdateDateFlag()) {
				//clearStartAndEndDate(temp);
			//}
		} else {
			temp.initializeItem();
		}
		item = temp;
  //}
}

Parser::~Parser(void) {
}