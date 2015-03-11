#pragma once
#include <string>
#include <sstream>
using namespace std;

class Parser {
private:
	string _fullUserInput;
	string _userCommand;
	string _commandData;
	int _lineOpNumber;

	int _day;
	int _month;
	//time saved in 24 _hours format
	int _hour;
	int _minute;
	int _duration;

public:
	Parser(string);

	//reset date, time and duration
	void resetDateTime();

	void setCommand(string);

	void setCommandData(string);

	void setEvent(string);

	void setDay(int);

	void setMonth(int);

	void setHour(int);

	void setMinute(int);

	void setLineOpNumber(int);

	string getUserCommand();

	string getCommandData();

	string getEvent();

	int getDay();

	int getMonth();

	int getHour();

	int getMinute();

	int getLineOpNumber();

	//extracts _userCommand and _commandData
	void extractUserCommand();

	size_t findFrontBracket(string);

	size_t findDateDelimiters(string);

	//input format is [_day/_month time(24hrs) at the end of the input
	//if time is input as 12 hr, input p to specify pm
	//12 is taken as 12 noon. if 12 am, then input m
	void extractDateAndTime();

	bool isValidDate();

	bool isValidTime();

	void separateDayMonth(string _day_month);

	void separateHourMinute(string _day_month);

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//the whitespaces with one less whitespace
	string removeSpacePadding(string);

	//Converts a number in string format to integer format
	//returns true if successful
	bool getIntegerLineNumber();

	int convertStringToInteger(string numberString);

	~Parser(void);
};

