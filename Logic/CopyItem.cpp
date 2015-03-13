#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Item.cpp"
#include "Command.h"

using namespace std;

//CONSTANTS
const std::string ERROR_COPY_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
const std::string SUCCESS_COPIED = "copied line: \"%s\" \n";

class CopyItem : public Command {
private:
	int _input;
	std::string _message;
	std::string _copiedData;

public:
	CopyItem() {
		_input = 0;
		_message = "";
		_copiedData = "";
	}

	CopyItem(const int input) {
		_input = input;
		_message = "";
		_copiedData = "";
	}

	~CopyItem() {
	}

	void executeAction(vector<ITEM>& vectorStore) {

		if(_input == 0) {
			_message = ERROR_COPY_INVALID_LINE_NUMBER;
		}
		else {
			_message = "";
			_copiedData = vectorStore[_input-1].event;
			vectorStore.push_back(vectorStore[_input-1]);
			
			char buffer[1000];

			sprintf_s(buffer, SUCCESS_COPIED.c_str(), _copiedData.c_str());
			_message = buffer;
		}
	}

	std::string getMessage() {
		return _message;
	}

	std::string getCopiedData() {
		return _copiedData;
	}
};
