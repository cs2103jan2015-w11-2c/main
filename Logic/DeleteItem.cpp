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

public:
	DeleteItem() {
		_lineNumber = 0;
		_message = "";
	}

	DeleteItem(const int input) {
		_lineNumber = input;
		_message = "";
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
			Item deletedData;
			deletedData = (vectorStore[_lineNumber - 1]);
			vectorStore.erase(vectorStore.begin() + (_lineNumber - 1));

			char buffer[1000];

			sprintf_s(buffer, SUCCESS_DELETED.c_str(), deletedData.toString().c_str());
			_message = buffer;
		}
	}

	string getMessage() {
		return _message;
	}
};
