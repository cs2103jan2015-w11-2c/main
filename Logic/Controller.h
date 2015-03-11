#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "Parser.h"
#include "FileStorage.h"
#include "Command.cpp"
#include "AddItem.cpp"
#include "DeleteItem.cpp"
#include "ClearItems.cpp"
#include "SortAlphabetical.cpp"
#include "CopyItem.cpp"

using namespace std;

class Controller {
private:
	static const string SUCCESS_CLEARED;
	static const string SUCCESS_EDITED;
	static const string SUCCESS_FILENAME_CHANGED;
	static const string SUCCESS_FILE_LOCATION_CHANGED;
	static const string ERROR_INVALID_COMMAND;
	static const string ERROR_INVALID_LINE_NUMBER;
	static const string ERROR_FILE_EMPTY;
	static const string ERROR_SEARCH_ITEM_NOT_FOUND;
	static const string ERROR_FILE_OPERATION_FAILED;
	static const string ERROR_NO_FILENAME;
	static const string ERROR_FILE_ALREADY_EXISTS;
	static const string ERROR_FILEPATH_NOT_FOUND;

	static string MENU;
	static char buffer[1000];

	FileStorage outputFile;
	Parser *parser;
	vector<string> vectorStore;

	//TO BE CHANGED
	string fileName;

	//To be passed to the GUI
	string inputBoxMessage;
	string successMessage;

	//for edit function, to check if it is the initial edit call
	bool isFirstCommandCall;
	int lineNumberOperation;
	

public:
	Controller(void);

	string executeCommand(string);

	//API for UI (Main Text Box)
	string getInputBoxMessage();

	//API for UI (Message Box)
	string getSuccessMessage();

	void setInputBoxMessage(string);

	void setSuccessMessage(string);

	void initializeVector();

	bool rewriteFile();

	void commandOptions(string);

	void addData(string sentence, int, int , int, int);

	//returns the data deleted or *#*#*#*#* if not found
	void deleteData();

	//returns line number for operation or 0 if line number is invalid
	int getLineNumberForOperation();

	string displayAll();

	void clearAll();

	void sortAlphabetical();

	void selectionSortIgnoreCase();

	string getLowerCaseString(string);

	void swap(string& string1, string& string2);

	void search(string);

	void copy();

	void edit();

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
