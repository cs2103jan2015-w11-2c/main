#pragma once

#include <iostream>
#include <vector>
#include "FileStorage.h"
#include "Item.h"
#include "Command.h"

using namespace std;

class RenameFile : public Command {
private:
	string _fileName;
	string _message;

public:
	RenameFile() {
		_fileName = "save.txt";
		_message = "";
	}

	RenameFile(const string fileName) {
		_fileName = fileName;
		_message = "";
	}

	~RenameFile() {
	}

	void executeAction(FileStorage *outputFile) {
		if(_fileName == "") {
			_message = ERROR_NO_FILENAME;
		} else {
			int dotPos = _fileName.length() - 4;
			if(_fileName[dotPos] != '.') { //ensure that the file is a .txt file
				_fileName = _fileName + ".txt";
			}
			if(outputFile->changeFileName(_fileName)) {
				char buffer[1000];
				sprintf_s(buffer, SUCCESS_FILENAME_CHANGED.c_str(), outputFile->getFileName().c_str());
				_message = buffer;
			} else {
				_message = ERROR_FILE_ALREADY_EXISTS;
			}
		}
	}

	string getMessage() {
		return _message;
	}
};
