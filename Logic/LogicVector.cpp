#include "LogicVector.h"

//TO BE CHANGED!
const string LogicVector::SUCCESS_ADDED = "Added line: \"%s\"\n";
const string LogicVector::SUCCESS_DELETED = "Deleted line: \"%s\"\n";
const string LogicVector::SUCCESS_CLEARED = "All content deleted\n";
const string LogicVector::SUCCESS_SORTED = "All content sorted alphabetically\n";
const string LogicVector::ERROR_INVALID_COMMAND = "Invalid command specified! please try again\n";
const string LogicVector::ERROR_FILE_EMPTY = "File is empty\n";
const string LogicVector::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
const string LogicVector::ERROR_SEARCH_ITEM_NOT_FOUND = "\"%s\" Not found!\n";
const string LogicVector::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";


char LogicVector::buffer[250];


LogicVector::LogicVector(void) {
	initializeVector();
}

void LogicVector::initializeVector() {
	vectorStore = outputFile.getAllFileData();
}

bool LogicVector::rewriteFile() {
	outputFile.clearFile();
	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		outputFile.addLine(vectorStore[i]);
	}
	return true;
}


string LogicVector::executeCommand(string inputText) {
	string userCommand = extractUserCommand(inputText);
	textFromUser = inputText;
	string successMessage;
	if (userCommand == "display") {
		successMessage = "display";
	} else if (userCommand == "add") {
		successMessage = addData(textFromUser);
	} else if (userCommand == "delete") {
		successMessage = deleteData(textFromUser);
	} else if (userCommand == "clear") {
		successMessage = clearAll();
	} else if (userCommand == "sort") {
		successMessage = sortAlphabetical();
	} else if (userCommand == "search") {
		successMessage = search();
	} else if (userCommand == "rename") {
		successMessage = rename(textFromUser);
	} else if (userCommand == "move") {
		successMessage = move(textFromUser);
	} else if (userCommand == "exit") {
		successMessage = "exit";
	}

	return successMessage;
}

void LogicVector::commandOptions(string command) {

}


string LogicVector::extractUserCommand(string& inputText) {
	string userCommand;
	inputText = removeSpacePadding(inputText);
	size_t spacePos = inputText.find_first_of(" ");
	if (spacePos == string::npos) {
		userCommand = inputText;
		inputText = "";
	} else {
		userCommand = inputText.substr(0, spacePos);
		inputText = inputText.substr(spacePos);
		spacePos = inputText.find_first_not_of(" ");
		inputText = inputText.substr(spacePos);
	}
	return userCommand;
}

string LogicVector::removeSpacePadding(string line) {
	size_t end = line.find_last_not_of(" ");
	size_t start = line.find_first_not_of(" ");
	if (start != string::npos) {
		return line.substr(start, (end - start + 1));
	} else if (line == "") {
		return "";
	} else {
		return line.substr(1);
	}
}

string LogicVector::addData(string sentence) {
	vectorStore.push_back(sentence);
	outputFile.addLine(sentence);
	return getSuccessMessage("added");
}

string LogicVector::deleteData(string stringLineNumber) {
	int lineNumber;
	bool haveNumber = convertStringToInteger(stringLineNumber, lineNumber);
	if (haveNumber && lineNumber > 0 && lineNumber <= vectorStore.size()) {
		string dataToBeDeleted = (vectorStore[lineNumber - 1]);
		vectorStore.erase(vectorStore.begin() + (lineNumber - 1));
		if(rewriteFile()) {
			return getSuccessMessage("deleted", dataToBeDeleted);
		} else {
			return ERROR_FILE_OPERATION_FAILED;
		}
	} else {
		return ERROR_INVALID_LINE_NUMBER;
	}
}


bool LogicVector::convertStringToInteger(string stringLineNumber, int& lineNumber) {
	char *end;
	lineNumber = (int)strtol(stringLineNumber.c_str(), &end, 10);
	return (*end == 0);
}

string LogicVector::displayAll() {
	ostringstream oss;
	if (vectorStore.empty()) {
		//printErrorMessage("empty");
		return ("");
	}

	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		oss << (i + 1) << ". " << vectorStore[i] << endl << endl;
	}

	return oss.str();
}

string LogicVector::clearAll() {
	vectorStore.clear();
	if(outputFile.clearFile()) {
		return getSuccessMessage("cleared");
	} else {
		return ERROR_FILE_OPERATION_FAILED;
	}
}

string LogicVector::sortAlphabetical() {
	if (vectorStore.empty()) {
		return getErrorMessage("empty");
	} else {
		selectionSortIgnoreCase();
		if(rewriteFile()) {
			return getSuccessMessage("sorted");
		} else {
			return ERROR_FILE_OPERATION_FAILED;
		}
	}
}

void LogicVector::selectionSortIgnoreCase() {
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

string LogicVector::getLowerCaseString(string currentString) {
	transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
	return currentString;
}

void LogicVector::swap(string& string1, string& string2) {
	string tempString = string1;
	string1 = string2;
	string2 = tempString;
}

string LogicVector::search() {
	ostringstream oss;
	transform(textFromUser.begin(), textFromUser.end(), textFromUser.begin(), ::tolower);
	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		string currentString = vectorStore[i];
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		size_t position = currentString.find(textFromUser);
		if (position != string::npos) {
			oss << (i + 1) << ". " << vectorStore[i] << endl << endl;
		}
	}

	if (oss.str() == "") {
		return getErrorMessage("not found");
	}
	return oss.str();
}

string LogicVector::rename(string newFileName) {
	if(newFileName == "") {
		return "No filename specified!";
	} else {
		int dotPos = newFileName.length() - 4;
		if(newFileName[dotPos] != '.') { //ensure that the file is a .txt file
			newFileName = newFileName + ".txt";
		}
		if(outputFile.changeFileName(newFileName)) {
			string successMsg = "Filename changed to : " + outputFile.getFileName();
			return successMsg;
		} else {
			return "A file with the same name already exists!";
		}
	}
}

string LogicVector::move(string newFileLocation) {
	if(outputFile.changeFileLocation(newFileLocation)) {
		string successMsg = "File location changed to: " + outputFile.getFullFileName();
		return successMsg;
	} else {
		return "File path does not exist or file already exists";
	}
}


string LogicVector::getSuccessMessage(string successType, string description) {
	if (successType == "added") {
		sprintf_s(buffer, SUCCESS_ADDED.c_str(), textFromUser.c_str());
	} else if (successType == "deleted") {
		sprintf_s(buffer, SUCCESS_DELETED.c_str(), description.c_str());
	} else if (successType == "cleared") {
		sprintf_s(buffer, SUCCESS_CLEARED.c_str());
	} else if (successType == "sorted") {
		sprintf_s(buffer, SUCCESS_SORTED.c_str());
	}

	return buffer;
}

string LogicVector::getErrorMessage(string errorType, string sentence) {
	string errorMessage = "";
	if (errorType == "invalid command") {
		errorMessage = ERROR_INVALID_COMMAND;
	} else if (errorType == "empty") {
		errorMessage = ERROR_FILE_EMPTY;
	} else if (errorType == "invalid number") {
		errorMessage = ERROR_INVALID_LINE_NUMBER;
	} else if (errorType == "not found") {
		sprintf_s(buffer, ERROR_SEARCH_ITEM_NOT_FOUND.c_str(), sentence.c_str());
		errorMessage = buffer;
	}
	return errorMessage;
}

string LogicVector::getHelp() {
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

vector<string> LogicVector::getVectorStore() {
	return vectorStore;
}

LogicVector::~LogicVector(void) {
}
