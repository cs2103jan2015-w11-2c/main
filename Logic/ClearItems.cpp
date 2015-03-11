#pragma once

#include <iostream>
#include <vector>
#include "Command.cpp"

using namespace std;

//CONSTANTS

class ClearItems : public Command {
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
};
