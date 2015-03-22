#pragma once

#include <iostream>
#include <vector>
#include "FileStorage.h"
#include "Item.h"
#include "Command.h"

using namespace std;

class MoveFileLocation : public Command {
private:
	string _fileLocation;
	string _message;

public:
	MoveFileLocation() {
		_fileLocation = "";
		_message = "";
	}

	MoveFileLocation(const string fileLocation) {
		_fileLocation = fileLocation;
		_message = "";
	}

	~MoveFileLocation() {
	}

	void executeAction(FileStorage *outputFile) {
		if(outputFile->changeFileLocation(_fileLocation)) {
			char buffer[1000];
			sprintf_s(buffer, SUCCESS_FILE_LOCATION_CHANGED.c_str(), outputFile->getFullFileName().c_str());
			_message = buffer;
		} else {
			_message = ERROR_FILEPATH_NOT_FOUND;
		}
	}

	string getMessage() {
		return _message;
	}
	
	void negateAction(vector<Item> &vectorStore) {
	}
};
