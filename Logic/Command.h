#pragma once
#include <iostream>
#include <vector>
#include "Item.h"

using namespace std;

	static const string SUCCESS_ADDED  = "Added line: \"%s\"\n";
	static const string SUCCESS_DELETED = "Deleted line: \"%s\"\n";
	static const string SUCCESS_CLEARED = "All content deleted!\n";
	static const string SUCCESS_COPIED = "copied line: \"%s\" \n";
	static const string ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
	static const string ERROR_FILE_EMPTY  = "File is empty\n";
	static const string SUCCESS_SORTED = "All content sorted alphabetically\n";

class Command {
public:
	Command();
	~Command();
	virtual void executeAction(vector<Item>&);
	//virtual void executeAction(string&);
	virtual string getMessage();
};