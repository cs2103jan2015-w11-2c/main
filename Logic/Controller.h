#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "Parser.h"
#include "FileStorage.h"
using namespace std;

class Controller {
private:
	static const string SUCCESS_ADDED;
	static const string SUCCESS_DELETED;
	static const string SUCCESS_CLEARED;
	static const string SUCCESS_SORTED;
	static const string SUCCESS_COPIED;
	static const string SUCCESS_EDITED;
	static const string SUCCESS_FILENAME_CHANGED;
	static const string SUCCESS_FILE_LOCATION_CHANGED;
	static const string ERROR_INVALID_COMMAND;
	static const string ERROR_FILE_EMPTY;
	static const string ERROR_INVALID_LINE_NUMBER;
	static const string ERROR_SEARCH_ITEM_NOT_FOUND;
	static const string ERROR_FILE_OPERATION_FAILED;
	static const string ERROR_NO_FILENAME;
	static const string ERROR_FILE_ALREADY_EXISTS;
	static const string ERROR_FILEPATH_NOT_FOUND;

	static string MENU;
	static char buffer[250];

	FileStorage outputFile;
	Parser *parser;
	vector<string> vectorStore;

	//TO BE CHANGED
	string fileName;

public:
	Controller(void);

	void initializeVector();

	bool Controller::rewriteFile();

	string executeCommand(string);

	void commandOptions(string);
	
	string addData(string sentence);
	
	//returns the data deleted or *#*#*#*#* if not found
	string deleteData();

	string displayAll();

	string clearAll();

	string sortAlphabetical();

	void selectionSortIgnoreCase();

	string getLowerCaseString(string);

	void swap(string& string1, string& string2);

	string search(string);

	string copy();

	string edit();

	//NEED TO IMPLEMENT A textfile to reflect the change
	//in name so that the next time the program is run
	//it will not revert to old file name
	string rename(string newFileName);

	//Example of new file path:
	//C:\Users\Username\My Documents
	string move(string newFileLocation);

	string getSuccessMessage(string successType, string description = "");

	string getHelp();

	vector<string> getVectorStore();

	~Controller(void);
};

