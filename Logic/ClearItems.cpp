#pragma once

#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"

using namespace std;

class ClearItems : public Command {
public:
	ClearItems() {
	}

	~ClearItems() {
	}

	void executeAction(vector<Item>& vectorStore) {
		vectorStore.clear();
	}

	string getMessage() {
		return SUCCESS_CLEARED;
	}
};
