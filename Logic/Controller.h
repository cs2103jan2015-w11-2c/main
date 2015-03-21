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
	vector<RESULT> _todayResult;
	vector<RESULT> _otherResult;


	//To be passed to the GUI
	string _inputBoxMessage;
	string _successMessage;

	//for edit function, to check if it is the initial edit call
	bool _isFirstCommandCall;
	int _lineNumberOperation;
	

public:
	Controller(void);

	void executeCommand(string);

	//API for UI (Main Text Box)
	string getInputBoxMessage();

	//API for UI (Message Box)
	string getSuccessMessage();

	void setInputBoxMessage(string);

	void setSuccessMessage(string);

	void initializeVector();

	void generateResults(vector<Item>);

	bool rewriteFile();

	void commandOptions(string);

	void addData(Item);

	//returns the data deleted or *#*#*#*#* if not found
	void deleteData();

	//returns line number for operation or 0 if line number is invalid
	int getLineNumberForOperation();

	void displayAll();

	void clearAll();

	void sortAlphabetical();

	void search(Item);

	void copy(Item);

	void edit(Item);

	//NEED TO IMPLEMENT A textfile to reflect the change
	//in name so that the next time the program is run
	//it will not revert to old file name
	void rename(string newFileName);

	//Example of new file path:
	//C:\Users\Username\My Documents
	void move(string newFileLocation);

	string getHelp();

	vector<RESULT> getTodayResult();

	vector<RESULT> getOtherResult();

	void swap(Item&, Item&);

	int compareEarlierThan(const Item, const Item);

	void chronoSort(vector<Item>&);

	~Controller(void);
};
