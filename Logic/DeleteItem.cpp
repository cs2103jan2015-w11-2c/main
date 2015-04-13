#pragma once

//@author A0116179B

#include <iostream>
#include <vector>
#include <cassert>
#include "Item.h"
#include "Command.h"
#include "FileStorage.h"

using namespace std;

class DeleteItem : public Command {
private:
	vector<int> _lineNumbers;
	string _message;
	vector<Item> _deletedItems;
	bool _isMarkDone;
	FileStorage *_outputFile;
public:
	DeleteItem() {
		_message = "";
		_isMarkDone = false;
		_outputFile = FileStorage::getInstance();
	}

	DeleteItem(const vector<int> input, const bool isMarkDone, FileStorage * outputFile) {
		_lineNumbers = input;
		_message = "";
		_isMarkDone = isMarkDone;
		_outputFile = outputFile;
	}

	~DeleteItem() {
	}

	vector<Item> getMarkedItems() {
		return _deletedItems;
	}

	bool isMatch(const Item item1, const Item item2) {
		if (item1.event != item2.event) {
			return false;
		}
		for (int i = 0; i < 3; i++) {
			if (item1.eventDate[i] != item2.eventDate[i]) {
				return false;
			} 
			if (item1.eventEndDate[i] != item2.eventEndDate[i]) {
				return false;
			} 
		}
		for (int i = 0; i < 2; i++) {
			if (item1.eventStartTime[i] != item2.eventStartTime[i]) {
				return false;
			} 
			if (item1.eventStartTime[i] != item2.eventStartTime[i]) {
				return false;
			} 
		}	
		return true;
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

			if (_isMarkDone) {
				sprintf_s(buffer, SUCCESS_DONE.c_str(), oss.str().c_str());
			} else {
				sprintf_s(buffer, SUCCESS_DELETED.c_str(), oss.str().c_str());
			}
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
		if (_isMarkDone) {
			vector<Item> temp = _outputFile->getArchiveData();
			if (!temp.empty()) {
				_outputFile->clearArchive();
			} 
			for (unsigned int i = 0; i < temp.size(); i++) {
				for (unsigned int j = 0; j < _deletedItems.size(); j++) {
					if (!isMatch(temp[i], _deletedItems[j])) {
						_outputFile->addLineToArchive(temp[i]);
					}
				}
			}
		}
		for (unsigned int i = 0; i < _deletedItems.size(); i++) {
			vectorStore.push_back(_deletedItems[i]);
		}

		_deletedItems.clear();
	}
};
