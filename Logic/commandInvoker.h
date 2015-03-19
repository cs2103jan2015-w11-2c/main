#pragma once

#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"
#include "AddItem.cpp"
#include "DeleteItem.cpp"
#include "ClearItems.cpp"
#include "SortAlphabetical.cpp"
#include "CopyItem.cpp"
#include "SearchItem.cpp"
#include "EditItem.cpp"
#include "SortChronological.cpp"

class CommandInvoker
{
public:
	CommandInvoker(void);
	~CommandInvoker(void);
	void executeCommand(vector<Item>&, Command *, string&);
};

