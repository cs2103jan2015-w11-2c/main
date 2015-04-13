#pragma once

//@author A0116179B
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
		size_t illegalChars = _fileName.find_first_of ("!@#$*^&/\\()[]\"|;=,");
		if(illegalChars != string::npos) {
			throw std::out_of_range(ERROR_INVALID_FILENAME);
		}
		if(_fileName == "") {
			throw std::out_of_range(ERROR_NO_FILENAME);
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
				throw std::out_of_range(_message = ERROR_FILE_ALREADY_EXISTS);
			}
		}
	}

	string getMessage() {
		return _message;
	}
};
