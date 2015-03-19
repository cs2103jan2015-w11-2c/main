#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "DateTime.cpp"
#include "Parser.h"
#include "FileStorage.h"
#include "Item.h"
#include "CommandInvoker.h"

using namespace std;

struct RESULT {
	int lineNumber;
	string date, event, time;
};

class Controller {
private:
	static const string SUCCESS_FILENAME_CHANGED;
	static const string SUCCESS_FILE_LOCATION_CHANGED;
	static const string ERROR_FILE_OPERATION_FAILED;
	static const string ERROR_NO_FILENAME;
	static const string ERROR_FILE_ALREADY_EXISTS;
	static const string ERROR_FILEPATH_NOT_FOUND;

	static string _MENU;
	static char _buffer[1000];

	FileStorage *_outputFile;
	Parser *_parser;
	CommandInvoker *_invoker;
	vector<Item> _vectorStore;

	//To be passed to the GUI
	string _inputBoxMessage;
	string _successMessage;

	//for edit function, to check if it is the initial edit call
	bool _isFirstCommandCall;
	int _lineNumberOperation;
	

public:
	Controller(void);

	vector<RESULT> executeCommand(string);

	//API for UI (Main Text Box)
	string getInputBoxMessage();

	//API for UI (Message Box)
	string getSuccessMessage();

	void setInputBoxMessage(string);

	void setSuccessMessage(string);

	void initializeVector();

	vector<RESULT> generateResults(vector<Item>);

	bool rewriteFile();

	Item initializeItem(string, int, int, int, int, int color = 7, bool bold = false);

	void commandOptions(string);

	vector<RESULT> addData(Item);

	//returns the data deleted or *#*#*#*#* if not found
	vector<RESULT> deleteData();

	//returns line number for operation or 0 if line number is invalid
	int getLineNumberForOperation();

	vector<RESULT> displayAll();

	vector<RESULT> clearAll();

	vector<RESULT> sortAlphabetical();

	void sortChronological(vector<Item> &);

	vector<RESULT> search(string);

	vector<RESULT> copy(Item);

	vector<RESULT> edit(Item);

	//NEED TO IMPLEMENT A textfile to reflect the change
	//in name so that the next time the program is run
	//it will not revert to old file name
	void rename(string newFileName);

	//Example of new file path:
	//C:\Users\Username\My Documents
	void move(string newFileLocation);

	string getSuccessMessage(string successType, string description = "");

	string getHelp();

	vector<Item> getVectorStore();

	~Controller(void);
};
