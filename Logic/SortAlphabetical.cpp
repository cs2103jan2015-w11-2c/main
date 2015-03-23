#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Item.h"
#include "Command.h"

using namespace std;

class SortAlphabetical : public Command {
private:
	string _message;

public:
	SortAlphabetical() {
		_message = "";
	}

	~SortAlphabetical() {
	}

	
	void swap(Item& item1, Item& item2) {
		Item tempItem = item1;
		item1 = item2;
		item2 = tempItem;
	}

	string getLowerCaseString(string currentString) {
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		return currentString;
	}

	void selectionSortIgnoreCase(vector<Item>& vectorStore) {
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

	void executeAction(vector<Item>& vectorStore) {
		if (vectorStore.empty()) {
			_message = ERROR_FILE_EMPTY;
		}
		else {
			selectionSortIgnoreCase(vectorStore);
			_message = SUCCESS_SORTED_ALPHA;
		}
	}

	string getMessage() {
		return _message;
	}
	
	void negateAction(vector<Item> &vectorStore) {
	}
};
