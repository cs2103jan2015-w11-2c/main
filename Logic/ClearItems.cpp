#pragma once

#include <iostream>
#include <vector>
#include "Command.cpp"

using namespace std;

//CONSTANTS
const string SUCCESS_CLEARED = "All content deleted!\n";

class ClearItems {
private:
	vector<string> _vectorStore;

public:
	ClearItems() {
	}

	~ClearItems() {
	}

	vector<string> executeAction() {
		_vectorStore.clear();

		return _vectorStore;
	}

	string getMessage() {
		return SUCCESS_CLEARED;
	}
};
