#pragma once
#include <string>
#include <sstream>
#include "DateTime.cpp"
using namespace std;

class Parser {
private:
	DateTime dateTime;
	string _fullUserInput;
	string _userCommand;
	string _commandData;
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

	//reset date, time and duration
	void resetDateTime();

	string getUserCommand();

	string getCommandData();

	string getEvent();

	int getDay();

	int getMonth();

	int getYear();

	int getHour();

	int getMinute();

	//returns the line number for operations such as delete
	int getLineOpNumber();

	//extracts _userCommand and stores the rest of the input in _commandData
	void extractUserCommand(string);

	size_t findFrontBracket(string);

	size_t findDateDelimiters(string);

	//input format is [_day/_month time(24hrs) at the end of the input
	//if time is input as 12 hr, input p to specify pm
	//12 is taken as 12 noon. if 12 am, then input m
	void extractDateAndTime(string);

	bool isValidDate();

	bool isValidTime();

	void separateDayMonth(string _day_month);

	void separateHourMinute(string _day_month);

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//an empty string
	string removeSpacePadding(string);

	//Converts the first number in the input string into an integer
	//Sets _lineOpNumber and returns true if successful
	bool haveValidLineNumber();

	int convertStringToInteger(string numberString);

	~Parser(void);
};

