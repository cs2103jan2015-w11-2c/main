#pragma once

#include <iostream>
#include <vector>
#include <Command.h>
#include <Item.h>
#include "AddItem.cpp"
#include "DeleteItem.cpp"
#include "ClearItems.cpp"
#include "SortAlphabetical.cpp"
#include "CopyItem.cpp"

class CommandInvoker
{
public:
	CommandInvoker(void);
	~CommandInvoker(void);
	void executeCommand(vector<Item>&, Command *);
};

