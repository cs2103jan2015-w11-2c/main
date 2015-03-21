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

class CommandInvoker
{
private:
	stack<Command> *_undo;
	stack<Command> *_redo;
public:
	CommandInvoker(void);
	~CommandInvoker(void);
	void executeCommand(vector<Item> &, Command *, string &);
	void executeCommand(FileStorage *, Command *, string &);
};

