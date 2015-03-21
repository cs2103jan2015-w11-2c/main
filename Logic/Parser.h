#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include "Item.h"
#include "DateTimeParser.h"

using namespace std;

class Parser {
private:
	static const string ERROR_NO_LINE_NUMBER;
	static const string ERROR_INVALID_LINE_NUMBER;

	Item _item;
	DateTimeParser _splitDateTime;

	string _userCommand;
	string _event;
	int _lineOpNumber;

public:
	Parser();

	Parser(string);

	void Parser::init();

	string getUserCommand();

	Item getItem();

	//returns the line number for operations such as delete
	//throws out_of_range exception if line number is invalid
	int getLineOpNumber();

	//extracts _userCommand and stores the rest of the input in _event
	void extractUserCommand(string);

	size_t findFrontBracket(string);

	//input format is [_day/_month time(24hrs) at the end of the input
	//if time is input as 12 hr, input p to specify pm
	//12 is taken as 12 noon. if 12 am, then input m
	//remove date and time data from commandData
	void extractDateAndTime(string);

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//an empty string
	string removeSpacePadding(string);

	string convertStringToLowerCase(string inputString);

	~Parser(void);
};

