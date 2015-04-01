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
	string _oldFileName;
	string _message;

public:
	RenameFile() {
		_fileName = "save.txt";
		_oldFileName = "";
		_message = "";
	}

	RenameFile(const string fileName) {
		_fileName = fileName;
		_oldFileName = "";
		_message = "";
	}

	~RenameFile() {
	}
	//@author A0111951N
	void executeAction(FileStorage *outputFile) {
		if(_fileName == "") {
			_message = ERROR_NO_FILENAME;
		} else {
			_oldFileName = outputFile->getFileName();
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
