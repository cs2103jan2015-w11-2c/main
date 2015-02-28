#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "FileStorage.h"
using namespace std;

class LogicVector {
private:
	static const string SUCCESS_ADDED;
	static const string SUCCESS_DELETED;
	static const string SUCCESS_CLEARED;
	static const string SUCCESS_SORTED;
	static const string ERROR_INVALID_COMMAND;
	static const string ERROR_FILE_EMPTY;
	static const string ERROR_INVALID_LINE_NUMBER;
	static const string ERROR_SEARCH_ITEM_NOT_FOUND;
	static const string ERROR_FILE_OPERATION_FAILED;
	static string MENU;
	static char buffer[250];

	FileStorage outputFile;
	vector<string> vectorStore;

	//TO BE CHANGED
	string fileName;

	//text after the command entered by the user
	string textFromUser;

public:
	LogicVector(void);

	void initializeVector();

	bool LogicVector::rewriteFile();

	string executeCommand(string);

	void commandOptions(string);

	//returns command and removes command from original string
	string extractUserCommand(string&);

	//remove leading and ending whitespace of string, if any
	//if only whitespaces are input, then it returns
	//the whitespaces with one less whitespace
	string removeSpacePadding(string);

	string addData(string sentence);
	
	//returns the data deleted or *#*#*#*#* if not found
	string deleteData(string);

	//Converts a number in string format to integer format
	//returns true if successful
	bool convertStringToInteger(const string, int&);

	string displayAll();

	string clearAll();

	string sortAlphabetical();

	void selectionSortIgnoreCase();

	string getLowerCaseString(string);

	void swap(string& string1, string& string2);

	string search();

	//NEED TO IMPLEMENT A textfile to reflect the change
	//in name so that the next time the program is run
	//it will not revert to old file name
	string rename(string newFileName);

	//Example of new file path:
	//C:\Users\Username\My Documents
	string move(string newFileLocation);

	string getSuccessMessage(string successType, string description = "");

	string getErrorMessage(string, string sentence = "");

	string getHelp();

	vector<string> getVectorStore();

	~LogicVector(void);
};

