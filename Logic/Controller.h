#pragma once

//@author A0116179B

#include <assert.h>
#include <exception>
#include <iostream>
#include <vector>
#include <algorithm>
#include "DateTime.h"
#include "Parser.h"
#include "FileStorage.h"
#include "Item.h"
#include "CommandInvoker.h"

using namespace std;

static const int NUM_HELP_COMMANDS = 12; 

static const string HELP_COMMANDS[] = {
	"add xxx", 
	"delete # {#, #-#}", 
	"display", 
	"edit # xxx",
	"copy # xxx",
	"clear", 
	"sort", 
	"search xxx",
	"12/24",
	"view/wide",
	"undo",
	"redo",
	"exit"};

static const string HELP_DESCRIPTION[] = {
	"line xxx is added to the text file with a line number",
	"the line with the corresponding # is deleted. (deletion of multiple line supported)",
	"all data in the file is displayed",
	"modifies the data in line # with xxx",
	"makes a copy of line #, and applies changes in time and date specified in xxx",
	"all data in the file is deleted",
	"all data in the file is sorted alphabetically",
	"all lines related to xxx are displayed",
	"Sets the time format to 12 Hour and 24 Hour respectively",
	"Widens the textbox to show more items",
	"Undo the previous change",
	"Redo the a change if undo was previously called",
	"program quits"
};

static const string DEADLINE_HEADER = "Deadline Events";
static const string ERROR_INCORRECT_NUMBER_ARGUMENTS = "Sleep requires 4 arguments\n";;

static const int DEFAULT_SLEEP_START_HR = 23;
static const int DEFAULT_SLEEP_START_MIN = 0;
static const int DEFAULT_SLEEP_END_HR = 5;
static const int DEFAULT_SLEEP_END_MIN = 0;

class Controller {
private:
	static const string SUCCESS_FILENAME_CHANGED;
	static const string SUCCESS_FILE_LOCATION_CHANGED;
	static const string SUCCESS_12_HR;
	static const string SUCCESS_24_HR;
	static const string SUCCESS_NOTIFICATION_TIME_CHANGED;
	static const string SUCCESS_NOTIFICATION_ON;
	static const string SUCCESS_NOTIFICATION_OFF;
	static const string ERROR_FILE_OPERATION_FAILED;
	static const string ERROR_INVALID_LINE_NUMBER;
	static const string ERROR_NO_FILENAME;
	static const string ERROR_FILE_ALREADY_EXISTS;
	static const string ERROR_FILEPATH_NOT_FOUND;
	static const string ERROR_INVALID_NOTIFICATION_TIME;
	static const int MAX_NOTIFICATION;

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

	//for option to allow widening of display
	bool _isWide;

	bool _isHelp;

	bool _is12HourFormat;

	int _sleepTime[2][2];

	//advanced notification time, in minutes
	//maximum allowable is 30240 (3 weeks)
	int _notifyTime;

	bool _isNotificationsOn;

public:
	Controller(void);

	void executeCommand(string);

	string getInputBoxMessage();

	string getSuccessMessage();

	void setInputBoxMessage(string);

	void setSuccessMessage(string);

	void initializeOptions();

	void initializeVector();

	long getTimePos(const int [3], const int [2]);

	bool checkDateIsUnset(const int [3]);

	bool checkIsClash(const Item, const Item);

	bool checkIsDeadline(const Item);

	bool checkIsExpired(const Item);

	bool checkIsFloating(const Item);

	void generateResults(const vector<Item>);

	bool rewriteFile();

	void commandOptions(string);

	void addData(Item);

	//returns the data deleted or *#*#*#*#* if not found
	void deleteData();

	void displayAll();

	void clearAll();

	void sortAlphabetical();

	void search(Item, string);

	void searchFree(Item, string);

	bool isSearch();

	void toggleIsWide();

	bool isWide();

	bool isHelp();

	void copy(Item);

	void edit(Item);

	void rename(string newFileName);

	//Example of new file path:
	//C:\Users\Username\My Documents
	void move(string newFileLocation);

	void undo();

	void redo();

	//command stored in date field
	//description stored in event field
	void getHelp();

	vector<RESULT> getTodayResult();

	vector<RESULT> getOtherResult();

	void swap(Item&, Item&);

	int compareEarlierThan(const Item, const Item);

	void chronoSort(vector<Item>&);

	void addToInputBank();

	vector<string> getInputBank();

	void setClockTo12Hour();

	void setClockTo24Hour();

	void setSleepTime();

	bool isNotificationsOn();

	string getNotifications();

	void calculateTargetDateTime(int&, int&, int&, int&, int&);

	string findEventMatch(int&, int&, int&, int&, int&);

	void setReminderTime();

	void toggleNotification();

	~Controller(void);
};
