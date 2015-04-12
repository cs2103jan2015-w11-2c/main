#pragma once

//@author A0111951N
#include <iostream>
#include <vector>
#include "FileStorage.h"
#include "Item.h"
#include "Command.h"

using namespace std;

class MoveFileLocation : public Command {
private:
	string _fileLocation;
	string _oldFileLocation;
	string _message;

public:
	MoveFileLocation() {
		_fileLocation = "";
		_oldFileLocation = "";
		_message = "";
	}

	MoveFileLocation(const string fileLocation) {
		_fileLocation = fileLocation;
		_oldFileLocation = "";
		_message = "";
	}

	~MoveFileLocation() {
	}

	void executeAction(FileStorage *outputFile) {
		_oldFileLocation = outputFile->getFileLocation();
		if(outputFile->changeFileLocation(_fileLocation)) {
			char buffer[1000];
			sprintf_s(buffer, SUCCESS_FILE_LOCATION_CHANGED.c_str(), outputFile->getFullFileName().c_str());
			_message = buffer;
		} else {
			throw std::invalid_argument(ERROR_FILEPATH_NOT_FOUND);
		}
	}

	string getMessage() {
		return _message;
	}
};
