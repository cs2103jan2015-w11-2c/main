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

public:
	Parser(string);

	void setCommand(string);

	void setCommandData(string);

	void setlineOpNumber(int);

	string getUserCommand();

	string getCommandData();

	int getLineOpNumber();

	//extracts userCommand and commandData
	void extractUserCommand();

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//the whitespaces with one less whitespace
	string removeSpacePadding(string);

	//Converts a number in string format to integer format
	//returns true if successful
	bool getIntegerLineNumber();

	~Parser(void);
};

