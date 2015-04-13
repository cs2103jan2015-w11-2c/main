#pragma once

//@author A0116179B

#include <iostream>
#include <vector>
#include "FileStorage.h"
#include "Item.h"

using namespace std;

static const string SUCCESS_ADDED  = "Added line: \"%s\"";
static const string SUCCESS_DELETED = "Deleted line number(s): %s";
static const string SUCCESS_DONE = "Line numbers marked as completed: %s";
static const string SUCCESS_CLEARED = "All content deleted!";
static const string SUCCESS_COPIED = "copied line: \"%s\" \n";
static const string SUCCESS_SORTED_ALPHA = "All content sorted alphabetically";
static const string SUCCESS_SORTED_CHRONO = "All content sorted chronologically";
static const string SUCCESS_EDITED = "\"%s\" changed to \"%s\"!";
static const string SUCCESS_SEARCH = "Search results for: \"%s\"";
static const string SUCCESS_FILENAME_CHANGED = "Filename changed to \"%s\"";
static const string SUCCESS_FILE_LOCATION_CHANGED = "File location changed to %s";
static const string ERROR_INVALID_LINE_NUMBER = "Invalid number specified:";
static const string ERROR_MISSING_EVENT = "No event specified!";
static const string ERROR_FILE_EMPTY  = "File is empty";
static const string ERROR_NO_FILENAME = "No filename specified!";
static const string ERROR_FILE_ALREADY_EXISTS = "A file with the same name already exists in the location specified";
static const string ERROR_NO_SEARCH_QUERY = "No search query entered!";
static const string ERROR_FILEPATH_NOT_FOUND = "The specified filepath was not found or the file already exists there";
static const string ERROR_SEARCH_ITEM_NOT_FOUND = "\"%s\" Not found!";
	
struct RESULT {
	string lineNumber;
	string date;
	string event;
	string time;
	bool isClash;
	bool isDeadline;
	bool isExpired;
};

class Command {
public:
	Command();
	~Command();
	virtual void executeAction(FileStorage *);	
	virtual void executeAction(vector<Item> &);
	virtual void negateAction(vector<Item> &);
	virtual string getMessage();
};