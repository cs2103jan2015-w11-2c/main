#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include "FileStorage.h"
#include "Item.h"
#include "Command.h"
#include "AddItem.cpp"
#include "DeleteItem.cpp"
#include "ClearItems.cpp"
#include "SortAlphabetical.cpp"
#include "CopyItem.cpp"
#include "SearchItem.cpp"
#include "EditItem.cpp"
#include "RenameFile.cpp"
#include "MoveFile.cpp"

//CONSTANTS
static const int MAX_UNDO = 20;
static const int MAX_REDO = 20;
static const string SUCCESS_UNDO = "Successful undo of: ";

class CommandInvoker
{
private:
	int _numRedo;
	bool _enableUndoFlag;
	stack<Command *> *_undo;
	stack<Command *> *_redo;
public:
	CommandInvoker(void);
	~CommandInvoker(void);
	void executeCommand(vector<Item> &, Command *, string &);
	void executeCommand(FileStorage *, Command *, string &);
	void disableUndo();
	void undo(vector<Item> &, string &);
	void redo(vector<Item> &, string &);
};

