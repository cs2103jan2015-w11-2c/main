#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Item.h"
#include "Command.h"

using namespace std;

class CopyItem : public Command {
private:
	int _input;
	std::string _message;

public:
	CopyItem() {
		_input = 0;
		_message = "";
	}

	CopyItem(const int input) {
		_input = input;
		_message = "";
	}

	~CopyItem() {
	}

	void executeAction(vector<Item>& vectorStore) {

		if(_input == 0) {
			_message = ERROR_INVALID_LINE_NUMBER;
		}
		else {
			_message = "";

			Item copiedData;
			copiedData = vectorStore[_input-1];
			vectorStore.push_back(vectorStore[_input-1]);
			
			char buffer[1000];

			sprintf_s(buffer, SUCCESS_COPIED.c_str(), copiedData.toString().c_str());
			_message = buffer;
		}
	}

	std::string getMessage() {
		return _message;
	}
};
