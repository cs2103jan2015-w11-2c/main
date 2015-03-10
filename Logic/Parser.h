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

	string eventToBeDone;
	string day;
	string month;
	string year;
	string time;

public:
	Parser(string);

	void setCommand(string);

	void setCommandData(string);

	void setEvent(string);

	void setDay(string);

	void setMonth(string);

	void setTime(string);

	void setLineOpNumber(int);

	//void extractAllData();

	string getUserCommand();

	string getCommandData();

	string getEvent();

	string getDay();

	string getMonth();

	string getTime();

	int getLineOpNumber();

	//extracts userCommand and commandData
	void extractUserCommand();

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//the whitespaces with one less whitespace
	string removeSpacePadding(string);

	//format of user input is [day month time]
	void extractDateAndTime(string eventTime);

	//Converts a number in string format to integer format
	//returns true if successful
	bool getIntegerLineNumber();

	~Parser(void);
};

