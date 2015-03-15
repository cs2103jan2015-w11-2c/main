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
	string _message;

public:
	SortAlphabetical() {
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

	void selectionSortIgnoreCase(vector<ITEM>& vectorStore) {
	for (unsigned int i = 0; i < (vectorStore.size() - 1); i++) {
		int minIndex = i;
		string minString = getLowerCaseString(vectorStore[minIndex].event);
		string minValue = vectorStore[minIndex].event;
		for (unsigned int j = i + 1; j < vectorStore.size(); j++) {
			string currString = getLowerCaseString(vectorStore[j].event);
			if (currString == minString && vectorStore[j].event < vectorStore[minIndex].event) {
				swap(vectorStore[minIndex], vectorStore[j]);
				minValue = vectorStore[minIndex].event;
			}
			if (currString < minString) {
				minString = getLowerCaseString(vectorStore[j].event);
				minValue = vectorStore[j].event;
				minIndex = j;
			}
		}
		swap(vectorStore[minIndex],vectorStore[i]);
	}
}

	void executeAction(vector<ITEM>& vectorStore) {
		if (vectorStore.empty()) {
			_message = ERROR_FILE_EMPTY;
		}
		else {
			selectionSortIgnoreCase(vectorStore);
			_message = SUCCESS_SORTED;
		}
	}

	string getMessage() {
		return _message;
	}
};
