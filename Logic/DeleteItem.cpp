#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"
#include "Command.h"

using namespace std;

class DeleteItem : public Command {
private:
	int _lineNumber;
	string _message;
	string _deletedData;

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
			_message = ERROR_INVALID_LINE_NUMBER;
		} else {
			_deletedData = (vectorStore[_lineNumber - 1].event);
			vectorStore.erase(vectorStore.begin() + (_lineNumber - 1));

			char buffer[1000];

			sprintf_s(buffer, SUCCESS_DELETED.c_str(), _deletedData.c_str());
			_message = buffer;
		}
	}

	string getMessage() {
		return _message;
	}
};
