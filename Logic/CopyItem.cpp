#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Command.cpp"

using namespace std;

//CONSTANTS
const string ERROR_COPY_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
const string SUCCESS_COPIED = "%s copied successfully!\n";

class CopyItem {
private:
	int _input;
	vector<string> _vectorStore;
	string _message;
	string _copiedData;

public:
	CopyItem() {
		_input=0;
		_message="";
		_copiedData="";
	}

	CopyItem(vector<string> vectorStore, const int input) {
		_input=input;
		_vectorStore=vectorStore;
		_message="";
		_copiedData="";
	}

	~CopyItem() {
	}

	vector<string> executeAction() {

		if(_input==0) {
			_message=ERROR_COPY_INVALID_LINE_NUMBER;
		}
		else {
			_message="";
			_copiedData = _vectorStore[_input-1];
			_vectorStore.push_back(_vectorStore[_input-1]);
			
			char buffer[1000];
			sprintf_s(buffer, SUCCESS_COPIED.c_str(), _copiedData.c_str());
			_message=buffer;
		}
		
		return _vectorStore;
	}

	string getMessage() {
		return _message;
	}

	string getCopiedData() {
		return _copiedData;
	}
};
