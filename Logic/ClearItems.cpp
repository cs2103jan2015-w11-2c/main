#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"

using namespace std;

//CONSTANTS
const string SUCCESS_CLEARED = "All content deleted!\n";

class ClearItems {
private:
	vector<ITEM> _vectorStore;

public:
	ClearItems() {
	}

	~ClearItems() {
	}

	vector<ITEM> executeAction() {
		_vectorStore.clear();

		return _vectorStore;
	}

	string getMessage() {
		return SUCCESS_CLEARED;
	}
};
