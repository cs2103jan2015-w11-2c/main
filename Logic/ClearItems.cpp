#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"
#include "Command.h"

using namespace std;

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
