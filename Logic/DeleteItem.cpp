#pragma once

//#define NDEBUG

#include <iostream>
#include <vector>
#include <cassert>
#include "Item.h"
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


	void executeAction(vector<Item>& vectorStore) {
		int lineToBeDeleted = _lineNumber;
		assert(lineToBeDeleted>=0);

		int sizeOfVectorStore = vectorStore.size();
		assert(sizeOfVectorStore>0);

		assert(lineToBeDeleted<=sizeOfVectorStore);

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
