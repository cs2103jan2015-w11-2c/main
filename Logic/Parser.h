#pragma once
#include <string>
#include <sstream>
#include <exception>
#include "DateTime.cpp"
using namespace std;


class Parser {
private:
	DateTime dateTime;
	string _fullUserInput;

	string _userCommand;
	string _event;
	int _lineOpNumber;

	int _day;
	int _month;
	int _year;

	//time saved in 24 _hours format
	int _hour;
	int _minute;
	int _duration;

public:
	Parser();

	Parser(string);

	void Parser::init();

	//reset date, time and duration
	void resetDateTime();

	string getUserCommand();

	string getEvent();

	int getDay();

	int getMonth();

	int getYear();

	int getHour();

	int getMinute();

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
	void extractDateAndTime(string);

	bool isValidTime();

	void separateDayMonth(string _day_month);

	void separateHourMinute(string _day_month);

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//an empty string
	string removeSpacePadding(string);

	int convertStringToInteger(string numberString);

	~Parser(void);
};

