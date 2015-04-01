#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include <map>
#include "Item.h"
#include "DateTimeParser.h"

using namespace std;

//Constants
const string DATE_KEYWORDS[]= {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday",
						"sunday", "mon", "tue", "tues", "wed", "thur", "thurs", "fri",
						"january", "february", "march", "april", "may", "june", "july",
						"august", "september", "october", "november", "december",
						"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", 
						"oct", "nov", "dec", "today", "floating", "float", "tomorrow", "tom",
						"from", "on", "to", "-", "next", "nex", "p", "pm", "m"};

class Parser {
private:
	static const string ERROR_NO_LINE_NUMBER;
	static const string ERROR_INVALID_LINE_NUMBER;

	Item _item;
	DateTimeParser _splitDateTime;

	string _userCommand;

public:
	Parser();

	void setStringToParse(string);

	string getUserCommand();

	Item getItem();

	//returns a vector of line numbers for operations such as delete
	//throws out_of_range exception if line number is invalid
	vector<int> getLineOpNumber();

	//extracts the first word of Item.event
	//and stores it in _userCommand
	void extractUserCommand();

	size_t findFrontBracket(string);

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

	vector<string> getFragmentedEvent();

	~Parser(void);
};

