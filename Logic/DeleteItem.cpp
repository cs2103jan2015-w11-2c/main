#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"
#include "Command.h"

using namespace std;

//CONSTANTS
const std::string SUCCESS_DELETED = "Deleted line: \"%s\"\n";
const std::string ERROR_DELETE_INVALID_LINE_NUMBER = "Invalid line number specified!\n";

class DeleteItem : public Command {
private:
	int _lineNumber;
	std::string _message;
	std::string _deletedData;

public:
	DeleteItem() {
		_lineNumber = 0;
		_message = "";
		_deletedData = "";
	}

	DeleteItem(const int input) {
		_lineNumber = input;
		_message = "";
		_deletedData = "";
	}

	~DeleteItem() {
	}


	void executeAction(vector<ITEM>& vectorStore) {
	
		if(_lineNumber == 0) {
			_message = ERROR_DELETE_INVALID_LINE_NUMBER;
		} else {
			_deletedData = (vectorStore[_lineNumber - 1].event);
			vectorStore.erase(vectorStore.begin() + (_lineNumber - 1));

			char buffer[1000];

			sprintf_s(buffer, SUCCESS_DELETED.c_str(), _deletedData.c_str());
			_message = buffer;
		}
	}

	std::string getMessage() {
		return _message;
	}
};
