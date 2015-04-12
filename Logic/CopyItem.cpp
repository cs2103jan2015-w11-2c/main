#pragma once

//@author A0116179B

#include <iostream>
#include <vector>
#include <string>
#include "Item.h"
#include "Command.h"

using namespace std;

class CopyItem : public Command {
private:
	int _lineNumber;
	Item _input;
	string _message;
	Item _copiedData;

public:
	CopyItem() {
		_lineNumber = 0;
		_message = "";
	}

	CopyItem(const int lineNumber, const Item input) {
		_lineNumber = lineNumber;
		_input = input;
		_message = "";
	}

	~CopyItem() {
	}

	void executeAction(vector<Item>& vectorStore) {

		if(_lineNumber == 0  || _lineNumber > (int)vectorStore.size()) {
			_message = ERROR_INVALID_LINE_NUMBER + " ";
			_message += ('0'+ _lineNumber);
			_message += "\n";
		}
		else {
			_message = "";

			_copiedData = vectorStore[_lineNumber - 1];
			_copiedData.eventDate[0] = _input.eventDate[0];
			_copiedData.eventDate[1] = _input.eventDate[1];
			_copiedData.eventDate[2] = _input.eventDate[2];
			_copiedData.eventStartTime[0] = _input.eventStartTime[0];
			_copiedData.eventStartTime[1] = _input.eventStartTime[1];
			_copiedData.eventEndTime[0] = _input.eventEndTime[0];
			_copiedData.eventEndTime[1] = _input.eventEndTime[1];
			
			vectorStore.push_back(_copiedData);
			
			char buffer[1000];

			sprintf_s(buffer, SUCCESS_COPIED.c_str(), _copiedData.toString().c_str());
			_message = buffer;
		}
	}

	string getMessage() {
		return _message;
	}

	bool isMatch(const Item item1, const Item item2) {
		if (item1.event != item2.event) {
			return false;
		}
		for (int i = 0; i < 3; i++) {
			if (item1.eventDate[i] != item2.eventDate[i]) {
				return false;
			}
		}
		for (int i = 0; i < 2; i++) {
			if (item1.eventStartTime[i] != item2.eventStartTime[i]) {
				return false;
			}
		}
		for (int i = 0; i < 2; i++) {
			if (item1.eventEndTime[i] != item2.eventEndTime[i]) {
				return false;
			}
		}
		return true;
	}

	void negateAction(vector<Item> &vectorStore) {
		for (unsigned int i = 0; i < vectorStore.size(); i++) {
			if (isMatch(vectorStore[i], _copiedData)) {
				vectorStore.erase(vectorStore.begin() + i);
				break;
			}
		}
	}
};
