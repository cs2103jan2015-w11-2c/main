#pragma once

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
	std::string _message;

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

		if(_lineNumber == 0) {
			_message = ERROR_INVALID_LINE_NUMBER;
		}
		else {
			_message = "";

			Item copiedData;
			copiedData = vectorStore[_lineNumber-1];
			copiedData.eventDate[0] = _input.eventDate[0];
			copiedData.eventDate[1] = _input.eventDate[1];
			copiedData.eventDate[2] = _input.eventDate[2];
			copiedData.eventStartTime[0] = _input.eventStartTime[0];
			copiedData.eventStartTime[1] = _input.eventStartTime[1];
			copiedData.eventEndTime[0] = _input.eventEndTime[0];
			copiedData.eventEndTime[1] = _input.eventEndTime[1];
			
			vectorStore.push_back(copiedData);
			
			char buffer[1000];

			sprintf_s(buffer, SUCCESS_COPIED.c_str(), copiedData.toString().c_str());
			_message = buffer;
		}
	}

	std::string getMessage() {
		return _message;
	}
};
