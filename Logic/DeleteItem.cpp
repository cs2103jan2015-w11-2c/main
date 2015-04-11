#pragma once

//@author A0111951N

#include <iostream>
#include <vector>
#include <cassert>
#include "Item.h"
#include "Command.h"

using namespace std;

class DeleteItem : public Command {
private:
	vector<int> _lineNumbers;
	string _message;
	vector<Item> _deletedItems;
public:
	DeleteItem() {
		_message = "";
	}

	DeleteItem(const vector<int> input) {
		_lineNumbers = input;
		_message = "";
	}

	~DeleteItem() {
	}


	void executeAction(vector<Item>& vectorStore) {
		bool isFirstError = true;
		bool isFirstSuccess= true;
		bool alreadyDeleted;
		int vectorSize = vectorStore.size();
		vector<int> deletedLines;
		ostringstream oss;

		_message = "";
		for (unsigned int i = 0; i < _lineNumbers.size(); i++) {
			int lineToBeDeleted = _lineNumbers[i];

			alreadyDeleted = false;
			for(unsigned int j = 0; j < deletedLines.size(); j++) {
				if (deletedLines[j] == lineToBeDeleted) {
					alreadyDeleted = true;
					break;
				} else {
					deletedLines.push_back(lineToBeDeleted);
				}
			}
			if (!alreadyDeleted) {
				if(lineToBeDeleted == 0 || lineToBeDeleted > vectorSize) {
					if(isFirstError) {
						_message = ERROR_INVALID_LINE_NUMBER + " ";
						isFirstError = false;
					} else {
						_message += ", ";
					}
					_message += ('0' + _lineNumbers[i]);
				} else {
					_deletedItems.push_back(vectorStore[lineToBeDeleted - 1 - i]);
					vectorStore.erase(vectorStore.begin() + (lineToBeDeleted - 1 - i));
					if (isFirstSuccess) {
						isFirstSuccess = false;
					} else {
						oss << ", ";
					}
					oss << lineToBeDeleted;
				}
			}
		}
		if (!isFirstSuccess) {
			char buffer[1000];

			sprintf_s(buffer, SUCCESS_DELETED.c_str(), oss.str().c_str());
			if(!isFirstError) {
				_message = ", " + _message;
			}
			_message = buffer + _message;
			_message += "\n";
		}
	}

	string getMessage() {
		return _message;
	}

	void negateAction(vector<Item> &vectorStore) {
		for (unsigned int i = 0; i < _deletedItems.size(); i++) {
			vectorStore.push_back(_deletedItems[i]);
		}
		_deletedItems.clear();
	}
};
