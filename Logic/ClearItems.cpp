#pragma once

//@author A0111951N

#include <iostream>
#include <vector>
#include "Item.h"
#include "Command.h"

using namespace std;

class ClearItems : public Command {
private:
	vector<Item> _vectorStore;
public:
	ClearItems() {
	}

	~ClearItems() {
	}

	void executeAction(vector<Item>& vectorStore) {
		_vectorStore = vectorStore;
		vectorStore.clear();
	}

	string getMessage() {
		return SUCCESS_CLEARED;
	}

	void negateAction(vector<Item> &vectorStore) {
		vectorStore = _vectorStore;
	}
};
