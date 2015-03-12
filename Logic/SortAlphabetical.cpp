#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Item.cpp"
#include "Command.h"

using namespace std;

const string SUCCESS_SORTED = "All content sorted alphabetically\n";
const string ERROR_FILE_EMPTY = "File is empty\n";

class SortAlphabetical : public Command {
private:
	vector<ITEM> _vectorStore;
	string _message;

public:
	SortAlphabetical() {
		_message = "";
	}

	SortAlphabetical(vector<ITEM> vectorStore) {
		_vectorStore=vectorStore;
		_message = "";
	}

	~SortAlphabetical() {
	}

	
	void swap(ITEM& item1, ITEM& item2) {
		ITEM tempItem = item1;
		item1 = item2;
		item2 = tempItem;
	}

	string getLowerCaseString(string currentString) {
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		return currentString;
	}

	void selectionSortIgnoreCase() {
	for (unsigned int i = 0; i < (_vectorStore.size() - 1); i++) {
		int minIndex = i;
		string minString = getLowerCaseString(_vectorStore[minIndex].event);
		string minValue = _vectorStore[minIndex].event;
		for (unsigned int j = i + 1; j < _vectorStore.size(); j++) {
			string currString = getLowerCaseString(_vectorStore[j].event);
			if (currString == minString && _vectorStore[j].event < _vectorStore[minIndex].event) {
				swap(_vectorStore[minIndex], _vectorStore[j]);
				minValue = _vectorStore[minIndex].event;
			}
			if (currString < minString) {
				minString = getLowerCaseString(_vectorStore[j].event);
				minValue = _vectorStore[j].event;
				minIndex = j;
			}
		}
		swap(_vectorStore[minIndex],_vectorStore[i]);
	}
}

	vector<ITEM> executeAction() {
		if (_vectorStore.empty()) {
			_message = ERROR_FILE_EMPTY;
		}
		else {
			selectionSortIgnoreCase();
			_message = SUCCESS_SORTED;
		}

		return _vectorStore;
	}

	string getMessage() {
		return _message;
	}
};
