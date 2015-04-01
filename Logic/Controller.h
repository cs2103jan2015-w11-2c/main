#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "DateTime.h"
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

	FileStorage *_outputFile;
	Parser *_parser;
	CommandInvoker *_invoker;
	vector<Item> _vectorStore;
	vector<RESULT> _todayResult;
	vector<RESULT> _otherResult;
	vector<string> _inputBank;


	//To be passed to the GUI
	string _inputBoxMessage;
	string _successMessage;

	//for search function, to check tell UI to change the heading to "Search Results"
	bool _isSearch;
	bool _isWide;

	//for option to allow widening of display
	bool _isWide;

public:
	Controller(void);

	void executeCommand(string);

	string getInputBoxMessage();

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

	bool isSearch();

	void toggleIsWide();

	bool isWide();

	void copy(Item);

	void edit(Item);

	//NEED TO IMPLEMENT A textfile to reflect the change
	//in name so that the next time the program is run
	//it will not revert to old file name
	void rename(string newFileName);

	//Example of new file path:
	//C:\Users\Username\My Documents
	void move(string newFileLocation);

	void undo();

	void redo();

	string getHelp();

	vector<RESULT> getTodayResult();

	vector<RESULT> getOtherResult();

	void swap(Item&, Item&);

	int compareEarlierThan(const Item, const Item);

	void chronoSort(vector<Item>&);

	void addToInputBank(const string);

	vector<string> getInputBank();

	~Controller(void);
};
