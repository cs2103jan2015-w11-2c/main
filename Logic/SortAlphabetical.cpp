#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Command.cpp"

using namespace std;

const string SUCCESS_SORTED = "All content sorted alphabetically\n";
const string ERROR_FILE_EMPTY = "File is empty\n";

class SortAlphabetical {
private:
	vector<string> _vectorStore;
	string _message;
public:
	SortAlphabetical() {
		_message="";
	}

	SortAlphabetical(vector<string> vectorStore) {
		_vectorStore=vectorStore;
		_message="";
	}

	~SortAlphabetical() {
	}

	
	void swap(string& string1, string& string2) {
		string tempString = string1;
		string1 = string2;
		string2 = tempString;
	}

	string getLowerCaseString(string currentString) {
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		return currentString;
	}

	void selectionSortIgnoreCase() {
	for (unsigned int i = 0; i < (_vectorStore.size() - 1); i++) {
		int minIndex = i;
		string minString = getLowerCaseString(_vectorStore[minIndex]);
		string minValue = _vectorStore[minIndex];
		for (unsigned int j = i + 1; j < _vectorStore.size(); j++) {
			string currString = getLowerCaseString(_vectorStore[j]);
			if (currString == minString && _vectorStore[j] < _vectorStore[minIndex]) {
				swap(_vectorStore[minIndex], _vectorStore[j]);
				minValue = _vectorStore[minIndex];
			}
			if (currString < minString) {
				minString = getLowerCaseString(_vectorStore[j]);
				minValue = _vectorStore[j];
				minIndex = j;
			}
		}
		_vectorStore[minIndex] = _vectorStore[i];
		_vectorStore[i] = minValue;
	}
}

	vector<string> executeAction() {
		if (_vectorStore.empty()) {
			_message=ERROR_FILE_EMPTY;
		}
		else {
			selectionSortIgnoreCase();
			_message=SUCCESS_SORTED;
		}

		return _vectorStore;
	}

	string getMessage() {
		return _message;
	}
};
