#pragma once
#include <iostream>
#include <vector>
#include "FileStorage.h"
#include "Item.h"

using namespace std;

	static const string SUCCESS_ADDED  = "Added line: \"%s\"\n";
	static const string SUCCESS_DELETED = "Deleted line numbers: %s";
	static const string SUCCESS_CLEARED = "All content deleted!\n";
	static const string SUCCESS_COPIED = "copied line: \"%s\" \n";
	static const string SUCCESS_SORTED_ALPHA = "All content sorted alphabetically\n";
	static const string SUCCESS_SORTED_CHRONO = "All content sorted chronologically\n";
	static const string SUCCESS_EDITED = "\"%s\" changed to \"%s\"!\n";
	static const string SUCCESS_SEARCH = "Search results for: \"%s\" \n";
	static const string SUCCESS_FILENAME_CHANGED = "Filename changed to \"%s\"\n";
	static const string SUCCESS_FILE_LOCATION_CHANGED = "File location changed to %s\n";
	static const string ERROR_INVALID_LINE_NUMBER = "Invalid line number specified:";
	static const string ERROR_FILE_EMPTY  = "File is empty\n";
	static const string ERROR_NO_FILENAME = "No filename specified!\n";
	static const string ERROR_FILE_ALREADY_EXISTS = "A file with the same name already exists in the location specified";
	static const string ERROR_FILEPATH_NOT_FOUND = "The specified filepath was not found or the file already exists there";
	static const string ERROR_SEARCH_ITEM_NOT_FOUND = "\"%s\" Not found!\n";
	

class Command {
public:
	Command();
	~Command();
	virtual void executeAction(FileStorage *);	
	virtual void executeAction(vector<Item>&);
	//virtual void executeAction(string&);
	virtual string getMessage();
};