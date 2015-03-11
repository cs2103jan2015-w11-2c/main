#pragma once

#include <iostream>
#include <vector>
#include "Controller.h"

using namespace std;

//CONSTANTS

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
};
