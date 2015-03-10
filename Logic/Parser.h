#pragma once
#include <string>
#include <sstream>
using namespace std;

class Parser {
private:
	string fullUserInput;
	string userCommand;
	string commandData;
	int lineOpNumber;

	int day;
	int month;
	//time saved in 24 hours format
	int hour;
	int minute;
	int duration;

public:
	Parser(string);

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

	//extracts userCommand and commandData
	void extractUserCommand();

	size_t findFrontBracket(string);

	size_t findDateDelimiters(string);

	void extractDateAndTime();

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//the whitespaces with one less whitespace
	string removeSpacePadding(string);

	//input format is [day/month time at the end of the input
	void extractDateAndTime(string eventTime);

	//Converts a number in string format to integer format
	//returns true if successful
	bool getIntegerLineNumber();

	int convertStringToInteger(string numberString);

	~Parser(void);
};

