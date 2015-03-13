#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"
#include "Command.h"

using namespace std;

//CONSTANTS
const string SUCCESS_CLEARED = "All content deleted!\n";

class ClearItems : public Command {
public:
	ClearItems() {
	}

	~ClearItems() {
	}

	void executeAction(vector<ITEM>& vectorStore) {
		vectorStore.clear();
	}

	string getMessage() {
		return SUCCESS_CLEARED;
	}
};
