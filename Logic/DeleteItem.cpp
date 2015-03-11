#pragma once

#include <iostream>
#include <vector>
#include "Item.cpp"

using namespace std;

//CONSTANTS
const std::string SUCCESS_DELETED = "Deleted line: \"%s\"\n";
const std::string ERROR_DELETE_INVALID_LINE_NUMBER = "Invalid line number specified!\n";

class DeleteItem {
private:
	int _lineNumber;
	vector<ITEM> _vectorStore;
	std::string _message;
	std::string _deletedData;

public:
	DeleteItem() {
		_lineNumber = 0;
		_message = "";
		_deletedData = "";
	}

	DeleteItem(vector<ITEM> vectorStore, const int input) {
		_lineNumber=input;
		_vectorStore=vectorStore;
		_message="";
		_deletedData="";
	}

	~DeleteItem() {
	}


	vector<ITEM> executeAction() {
	
		if(_lineNumber == 0) {
			_message = ERROR_DELETE_INVALID_LINE_NUMBER;
		} else {
			_deletedData = (_vectorStore[_lineNumber - 1].event);
			_vectorStore.erase(_vectorStore.begin() + (_lineNumber - 1));

			char buffer[1000];

			sprintf_s(buffer, SUCCESS_DELETED.c_str(), _deletedData.c_str());
			_message = buffer;
		}

		return _vectorStore;
	}

	std::string getMessage() {
		return _message;
	}
};