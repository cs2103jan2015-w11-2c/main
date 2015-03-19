#pragma once
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>
#include "Item.h"
#include "DateTime.cpp"

using namespace std;

class Parser {
private:
	Item _item;
	DateTime _dateTime;
	string _fullUserInput;

	string _userCommand;
	string _event;
	int _lineOpNumber;

public:
	Parser();

	Parser(string);

	void Parser::init();

	//reset date, time and duration
	void resetDateTime();

	void setDate(int day, int month, int year);

	string getUserCommand();

	string getEvent();

	Item getItem();

	//returns the line number for operations such as delete
	//throws out_of_range exception if line number is invalid
	int getLineOpNumber();

	//extracts _userCommand and stores the rest of the input in _event
	void extractUserCommand(string);

	size_t findFrontBracket(string);

	size_t findDateDelimiters(string);

	//input format is [_day/_month time(24hrs) at the end of the input
	//if time is input as 12 hr, input p to specify pm
	//12 is taken as 12 noon. if 12 am, then input m
	//remove date and time data from commandData
	void extractDateAndTime(string);

	void splitDateTime(string);

	void handleOneDateInput(string []);

	void handleTwoDateInput(string inputArray []);

	bool isValidTime(int, int);

	void separateDayMonthYear(string);

	void separateHourMinute(string, int&, int&);

	//ensure that date is correct. If not, throws an exception
	void verifyItemDate();

	void verifyItemTime(int&, int&);

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//an empty string
	string removeSpacePadding(string);

	int convertStringToInteger(string numberString);

	string convertStringToLowerCase(string inputString);

	~Parser(void);
};

