#pragma once
//@author A0111951N

#include <string>
#include <sstream>
#include <exception>
#include <assert.h>
#include <algorithm>
#include <map>
#include "Item.h"
#include "../EasyLoggingpp/easylogging++.h"
#include "DateTimeParser.h"

using namespace std;

//Constants
const string DATE_START_1 = "from";
const string DATE_START_2 = "on";
const string DATE_START_DEADLINE = "by";

class Parser {
private:
	static const string ERROR_NO_LINE_NUMBER;
	static const string ERROR_INVALID_LINE_NUMBER;
	static const string ERROR_SEARCH_STRING_EMPTY;
	static const string STRING_FLOATING;

	Item _item;
	DateTimeParser _dateTimeParse;
	string _userCommand;
	bool _isDeadline;
	bool _isDateUpdatedFromFloat;

public:
	Parser();

	void setStringToParse(string);

	string getUserCommand();

	Item getItem();

	//returns a vector of line numbers for operations such as delete
	//throws out_of_range exception if line number is invalid
	vector<int> getLineOpNumber();

	vector<int> getLineOpNumberForFree();

	//extracts the first word of Item.event
	//and stores it in _userCommand
	void extractUserCommand();

	//true if the event is a deadline event
	bool _isDeadlineEvent();

	//takes input string and keyword
	//returns the position of the delimiter, string::npos otherwise
	size_t findDateKeyWord(string, string);

	//returns true if the words after the delimiter are all date/time keywords
	bool isCorrectDateDelimiter(string inputLine, size_t index);

	//input format is [_day/_month time(24hrs) at the end of the input
	//if time is input as 12 hr, input p to specify pm
	//12 is taken as 12 noon. if 12 am, then input m
	//remove date and time data from commandData
	void extractDateAndTime();

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//an empty string
	string removeSpacePadding(string);

	string convertStringToLowerCase(string inputString);

	void assertItemValues();

	vector<string> getFragmentedEvent();

	bool checkIsFloating(const Item);

	void clearStartAndEndDate(Item &);

	bool isTimeOrDate(const string);

	bool checkIsValidDate(const string);

	bool checkIsDeadline(const string);

	void extractSearchQuery(Item &);

	bool getIsDateUpdatedFromFloat();

	~Parser(void);
};