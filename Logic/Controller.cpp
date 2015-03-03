#include "Controller.h"

//TO BE CHANGED!
const string Controller::SUCCESS_ADDED = "Added line: \"%s\"\n";
const string Controller::SUCCESS_DELETED = "Deleted line: \"%s\"\n";
const string Controller::SUCCESS_CLEARED = "All content deleted\n";
const string Controller::SUCCESS_SORTED = "All content sorted alphabetically\n";
const string Controller::SUCCESS_FILENAME_CHANGED = "Filename changed to %s\n";
const string Controller::SUCCESS_FILE_LOCATION_CHANGED = "File location changed to %s\n";
const string Controller::ERROR_INVALID_COMMAND = "Invalid command specified! please try again\n";
const string Controller::ERROR_FILE_EMPTY = "File is empty\n";
const string Controller::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
const string Controller::ERROR_SEARCH_ITEM_NOT_FOUND = "\"%s\" Not found!\n";
const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";
const string Controller::ERROR_NO_FILENAME = "No filename specified!\n";
const string Controller::ERROR_FILE_ALREADY_EXISTS = "A file with the same name already exists in the location specified";
const string Controller::ERROR_FILEPATH_NOT_FOUND = "The specified filepath was not found or the file already exists there";


char Controller::buffer[250];


Controller::Controller(void) {
	initializeVector();
}

void Controller::initializeVector() {
	vectorStore = outputFile.getAllFileData();
}

bool Controller::rewriteFile() {
	outputFile.clearFile();
	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		outputFile.addLine(vectorStore[i]);
	}
	return true;
}


string Controller::executeCommand(string inputText) {
	parser = new Parser(inputText);
	string userCommand = parser->getUserCommand();
	string commandData = parser->getCommandData();
	string successMessage;
	if (userCommand == "display") {
		successMessage = "display";
	} else if (userCommand == "add") {
		successMessage = addData(commandData);
	} else if (userCommand == "delete") {
		successMessage = deleteData();
	} else if (userCommand == "clear") {
		successMessage = clearAll();
	} else if (userCommand == "sort") {
		successMessage = sortAlphabetical();
	} else if (userCommand == "search") {
		successMessage = search(commandData);
	} else if (userCommand == "rename") {
		successMessage = rename(commandData);
	} else if (userCommand == "move") {
		successMessage = move(commandData);
	} else if (userCommand == "exit") {
		successMessage = "exit";
	}
	return successMessage;
}

void Controller::commandOptions(string command) {

}

string Controller::addData(string sentence) {
	vectorStore.push_back(sentence);
	outputFile.addLine(sentence);

	sprintf_s(buffer, SUCCESS_ADDED.c_str(), sentence.c_str());
	return buffer;
}

string Controller::deleteData() {
	bool validLineNumber = parser->getIntegerLineNumber();
	unsigned int lineNumber;
	if(validLineNumber) {
		lineNumber = parser->getLineOpNumber();
		if (lineNumber <= 0 || lineNumber > vectorStore.size()) {
			validLineNumber = false;
		}
	}

	if(!validLineNumber) {
		return ERROR_INVALID_LINE_NUMBER;
	} else {
		string dataToBeDeleted = (vectorStore[lineNumber - 1]);
		vectorStore.erase(vectorStore.begin() + (lineNumber - 1));
		if(rewriteFile()) {
			sprintf_s(buffer, SUCCESS_DELETED.c_str(), dataToBeDeleted.c_str());
			return buffer;
		} else {
			return ERROR_FILE_OPERATION_FAILED;
		}
	}
}

string Controller::displayAll() {
	ostringstream oss;
	if (vectorStore.empty()) {
		return ERROR_FILE_EMPTY;
	}

	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		oss << (i + 1) << ". " << vectorStore[i] << endl << endl;
	}

	return oss.str();
}

string Controller::clearAll() {
	vectorStore.clear();
	if(outputFile.clearFile()) {
		return SUCCESS_CLEARED;
	} else {
		return ERROR_FILE_OPERATION_FAILED;
	}
}

string Controller::sortAlphabetical() {
	if (vectorStore.empty()) {
		return ERROR_FILE_EMPTY;
	} else {
		selectionSortIgnoreCase();
		if(rewriteFile()) {
			return SUCCESS_SORTED;
		} else {
			return ERROR_FILE_OPERATION_FAILED;
		}
	}
}

void Controller::selectionSortIgnoreCase() {
	for (unsigned int i = 0; i < (vectorStore.size() - 1); i++) {
		int minIndex = i;
		string minString = getLowerCaseString(vectorStore[minIndex]);
		string minValue = vectorStore[minIndex];
		for (unsigned int j = i + 1; j < vectorStore.size(); j++) {
			string currString = getLowerCaseString(vectorStore[j]);
			if (currString == minString && vectorStore[j] < vectorStore[minIndex]) {
				swap(vectorStore[minIndex], vectorStore[j]);
				minValue = vectorStore[minIndex];
			}
			if (currString < minString) {
				minString = getLowerCaseString(vectorStore[j]);
				minValue = vectorStore[j];
				minIndex = j;
			}
		}
		vectorStore[minIndex] = vectorStore[i];
		vectorStore[i] = minValue;
	}
}

string Controller::getLowerCaseString(string currentString) {
	transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
	return currentString;
}

void Controller::swap(string& string1, string& string2) {
	string tempString = string1;
	string1 = string2;
	string2 = tempString;
}

string Controller::search(string searchText) {
	ostringstream oss;
	transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);
	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		string currentString = vectorStore[i];
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		size_t position = currentString.find(searchText);
		if (position != string::npos) {
			oss << (i + 1) << ". " << vectorStore[i] << endl << endl;
		}
	}

	if (oss.str() == "") {
		sprintf_s(buffer, ERROR_SEARCH_ITEM_NOT_FOUND.c_str(), searchText.c_str());
		return buffer;
	}
	return oss.str();
}

string Controller::rename(string newFileName) {
	if(newFileName == "") {
		return ERROR_NO_FILENAME;
	} else {
		int dotPos = newFileName.length() - 4;
		if(newFileName[dotPos] != '.') { //ensure that the file is a .txt file
			newFileName = newFileName + ".txt";
		}
		if(outputFile.changeFileName(newFileName)) {
			sprintf_s(buffer, SUCCESS_FILENAME_CHANGED.c_str(), outputFile.getFileName().c_str());
			return buffer;
		} else {
			return ERROR_FILE_ALREADY_EXISTS;
		}
	}
}

string Controller::move(string newFileLocation) {
	if(outputFile.changeFileLocation(newFileLocation)) {
		sprintf_s(buffer, SUCCESS_FILE_LOCATION_CHANGED.c_str(), outputFile.getFullFileName().c_str());
		return buffer;
	} else {
		return ERROR_FILEPATH_NOT_FOUND;
	}
}


string Controller::getSuccessMessage(string successType, string description) {
	if (successType == "added") {
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), description.c_str());
	} else if (successType == "deleted") {
		sprintf_s(buffer, SUCCESS_DELETED.c_str(), description.c_str());
	} else if (successType == "cleared") {
		sprintf_s(buffer, SUCCESS_CLEARED.c_str());
	} else if (successType == "sorted") {
		sprintf_s(buffer, SUCCESS_SORTED.c_str());
	}

	return buffer;
}

string Controller::getHelp() {
	ostringstream oss;
	oss << "Command:		Program execution:\n";
	oss << "add xxx		(line xxx is added to the text file with a line number)\n";
	oss << "delete #		(the line with the corresponding # is deleted)\n";
	oss << "display		(all data in the file is displayed)\n";
	oss << "clear			(all data in the file is deleted)\n";
	oss << "sort			(all data in the file sorted alphabetically)\n";
	oss << "search xxx		(all lines with xxx displayed)\n";
	oss << "exit			(program quits)\n";
	return oss.str();
}

vector<string> Controller::getVectorStore() {
	return vectorStore;
}

Controller::~Controller(void) {
}
