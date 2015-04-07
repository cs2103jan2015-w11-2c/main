#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include <map>
#include "Item.h"
#include "../EasyLoggingpp/easylogging++.h"
#include "DateTimeParser.h"

using namespace std;

//Constants
const string DATE_START_1 = "from";
const string DATE_START_2 = "on";
const int DATE_KEYWORDS_SIZE = 58;
const string DATE_KEYWORDS[]= {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday",
						"sunday", "mon", "tue", "tues", "wed", "thur", "thurs", "fri", "sat", "sun",
						"january", "february", "march", "april", "may", "june", "july", "august",
						 "september", "october", "november", "december", "jan", "feb", "mar",
						 "apr", "may", "jun", "jul", "aug", "sep", "sept", "oct", "nov", "novem",
						 "dec", "decem", "today", "floating", "float", "tomorrow", "tom", "tmr",
						 "from", "on", "to", "-", "next", "nex", "p", "pm", "m"};

class Parser {
private:
	static const string ERROR_NO_LINE_NUMBER;
	static const string ERROR_INVALID_LINE_NUMBER;
	static const string STRING_FLOATING;

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

	//takes input string and keyword
	//returns the position of the delimiter, string::npos otherwise
	size_t findDateKeyWord(string, string);

	//returns true if the words after the delimiter are all date/time keywords
	bool isCorrectDateDelimiter(string inputLine, size_t index);

	//returns true if the word is a possible date/time keyword
	bool isDateKeyword(string);

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

	bool checkIsFloating(const Item);

	void clearStartAndEndDate(Item &);

	void extractSearchQuery(Item &);

	~Parser(void);
};