#include "Controller.h"

//TO BE CHANGED!
const string Controller::SUCCESS_CLEARED = "All content deleted\n";
const string Controller::SUCCESS_EDITED = "\"%s\" changed to \"%s\"!\n";
const string Controller::SUCCESS_FILENAME_CHANGED = "Filename changed to \"%s\"\n";
const string Controller::SUCCESS_FILE_LOCATION_CHANGED = "File location changed to %s\n";
const string Controller::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
const string Controller::ERROR_FILE_EMPTY = "File is empty\n";
const string Controller::ERROR_SEARCH_ITEM_NOT_FOUND = "\"%s\" Not found!\n";
const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";
const string Controller::ERROR_NO_FILENAME = "No filename specified!\n";
const string Controller::ERROR_FILE_ALREADY_EXISTS = "A file with the same name already exists in the location specified";
const string Controller::ERROR_FILEPATH_NOT_FOUND = "The specified filepath was not found or the file already exists there";


char Controller::buffer[1000];


Controller::Controller(void) {
	initializeVector();
	isFirstCommandCall = true;
}

//API for UI (Main Text Box)
string Controller::getInputBoxMessage() {
	return inputBoxMessage;
}

//API for UI (Message Box)
string Controller::getSuccessMessage() {
	return successMessage;
}

void Controller::setInputBoxMessage(string message) {
	inputBoxMessage = message;
}

void Controller::setSuccessMessage(string message) {
	successMessage = message;
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
	//parser->setCommandData(inputText);
	parser->extractDateAndTime();
	int month = parser->getMonth();
	int day = parser->getDay();
	int hour = parser->getHour();
	int mins = parser->getMinute();

	if (userCommand == "display") {
		setSuccessMessage("display");
	} else if (userCommand == "add") {
		addData(commandData, month, day, hour, mins);
	} else if (userCommand == "delete") {
		deleteData();
	} else if (userCommand == "clear") {
		clearAll();
	} else if (userCommand == "sort") {
		sortAlphabetical();
	} else if (userCommand == "search") {
		search(commandData);
	} else if (userCommand == "copy") {
		copy();
	} else if (userCommand == "edit") {
		edit();
	} else if (userCommand == "rename") {
		rename(commandData);
	} else if (userCommand == "move") {
		move(commandData);
	} else if (userCommand == "exit") {
		setSuccessMessage("exit");
	}
	return getSuccessMessage();
}

void Controller::commandOptions(string command) {

}

void Controller::addData(string sentence, int month, int day, int hour, int mins) {
	ostringstream oss;

	oss << sentence << "[" << day << "/" << month << ", " << hour << "]";

	AddItem *addItemCommand = new AddItem(vectorStore, oss.str());
	vectorStore = addItemCommand->executeAction();

	outputFile.addLine(oss.str());

	setInputBoxMessage("");
	setSuccessMessage(addItemCommand->getMessage());
}

void Controller::deleteData() {
	string successMessage;

	DeleteItem *deleteItemCommand = new DeleteItem(vectorStore, getLineNumberForOperation());
	vectorStore=deleteItemCommand->executeAction();

	if(rewriteFile()) {
		setSuccessMessage(deleteItemCommand->getMessage());;
	} else {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	setInputBoxMessage("");
	
}

int Controller::getLineNumberForOperation() {
	bool validLineNumber = parser->getIntegerLineNumber();
	unsigned int lineNumber;
	if(validLineNumber) {
		lineNumber = parser->getLineOpNumber();
		if (lineNumber <= 0 || lineNumber > vectorStore.size()) {
			return 0;
		}
	}
	return lineNumber;
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

void Controller::clearAll() {
	ClearItems *clearItemsCommand = new ClearItems;
	
	vectorStore = clearItemsCommand->executeAction();

	if(outputFile.clearFile()) {
		setSuccessMessage(SUCCESS_CLEARED);
	} else {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
}

void Controller::sortAlphabetical() {
	
	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical(vectorStore);
	vectorStore=sortAlphabeticalCommand->executeAction();

	setInputBoxMessage("");
	setSuccessMessage(sortAlphabeticalCommand->getMessage());
}

void Controller::search(string searchText) {
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
		setSuccessMessage(buffer);
	}
	//to be changed to be shown in main outputbox
	setSuccessMessage(oss.str());
	setInputBoxMessage("");
}

void Controller::copy() {
	CopyItem *copyItemCommand = new CopyItem(vectorStore, getLineNumberForOperation());
	vectorStore=copyItemCommand->executeAction();

	string copiedData = copyItemCommand->getCopiedData();
	if(copiedData!="") {
		outputFile.addLine(copiedData);
		setSuccessMessage(copyItemCommand->getMessage());
	}
	setInputBoxMessage("");
	
}

void Controller::edit() {
	if(isFirstCommandCall) {
		int lineNumber = getLineNumberForOperation();
		if(lineNumber == 0) {
			setSuccessMessage(ERROR_INVALID_LINE_NUMBER);
			setInputBoxMessage("");
		} else {
			string lineToCopy = "edit " + vectorStore[lineNumber - 1];
			setSuccessMessage("");
			setInputBoxMessage(lineToCopy);
		}
		isFirstCommandCall = false;
		lineNumberOperation = lineNumber;
	} else {
		sprintf_s(buffer, SUCCESS_EDITED.c_str(), 
			vectorStore[lineNumberOperation - 1].c_str(), 
			parser->getCommandData().c_str());
		vectorStore[lineNumberOperation - 1] = parser->getCommandData();
		rewriteFile();
		isFirstCommandCall = true;
		setSuccessMessage(buffer);
		setInputBoxMessage("");
	}
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
	return successMessage;
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