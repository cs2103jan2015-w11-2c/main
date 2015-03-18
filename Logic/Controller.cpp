#include "Controller.h"
#include "easylogging++.h"

const string Controller::SUCCESS_EDITED = "\"%s\" changed to \"%s\"!\n";
const string Controller::SUCCESS_FILENAME_CHANGED = "Filename changed to \"%s\"\n";
const string Controller::SUCCESS_FILE_LOCATION_CHANGED = "File location changed to %s\n";
const string Controller::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
const string Controller::ERROR_SEARCH_ITEM_NOT_FOUND = "\"%s\" Not found!\n";
const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";
const string Controller::ERROR_NO_FILENAME = "No filename specified!\n";
const string Controller::ERROR_FILE_ALREADY_EXISTS = "A file with the same name already exists in the location specified";
const string Controller::ERROR_FILEPATH_NOT_FOUND = "The specified filepath was not found or the file already exists there";


char Controller::buffer[1000];

INITIALIZE_EASYLOGGINGPP

Controller::Controller(void) {
	initializeVector();
	_isFirstCommandCall = true;
}

string Controller::executeCommand(string inputText) {
	parser = new Parser(inputText);

	int month = parser->getMonth();
	int day = parser->getDay();
	int hour = parser->getHour();
	int mins = parser->getMinute();
	int colour = 7; //temp, until parser can set colours

	string userCommand = parser->getUserCommand();
	string commandData = parser->getEvent();

	Item data = initializeItem(commandData, day, month, hour, mins, colour);

	if (userCommand == "display") {
		setSuccessMessage("display");
	} else if (userCommand == "add") {
		addData(data);
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

//API for UI (Main Text Box)
string Controller::getInputBoxMessage() {
	return _inputBoxMessage;
}

//API for UI (Message Box)
string Controller::getSuccessMessage() {
	return _successMessage;
}

void Controller::setInputBoxMessage(string message) {
	_inputBoxMessage = message;
}

void Controller::setSuccessMessage(string message) {
	_successMessage = message;
}

void Controller::initializeVector() {
	//outputFile needs to be able to work with ITEM structure
	//vectorStore = outputFile.getAllFileData();
}

bool Controller::rewriteFile() {
	outputFile.clearFile();
	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		outputFile.addLine(vectorStore[i].event);
	}
	return true;
}

Item Controller::initializeItem(string event, int day, int month, int hour, int min, int col, bool bold) {
	Item temp;

	temp.event=event;
	temp.eventDate[0] = day;
	temp.eventDate[1] = month;
	temp.eventStartTime[0] = hour;
	temp.eventStartTime[1] = min;
	temp.colour = col;
	temp.bold = bold;

	return temp;
}

void Controller::commandOptions(string command) {

}

void Controller::addData(Item item) {
	AddItem *addItemCommand = new AddItem(item);

	addItemCommand->executeAction(vectorStore);

	outputFile.addLine(item.event);

	setInputBoxMessage("");
	setSuccessMessage(addItemCommand->getMessage());
}

void Controller::deleteData() {
	string successMessage;

	DeleteItem *deleteItemCommand = new DeleteItem(getLineNumberForOperation());
	deleteItemCommand->executeAction(vectorStore);

	if(rewriteFile()) {
		setSuccessMessage(deleteItemCommand->getMessage());
	} else {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
	
	setInputBoxMessage("");
	
}

int Controller::getLineNumberForOperation() {
	unsigned int lineNumber = 0;
	try {
		lineNumber = parser->getLineOpNumber();
		if (lineNumber <= 0 || lineNumber > vectorStore.size()) {
			return 0;
		}
	} catch (const out_of_range& e) {
		LOG(ERROR) << "getLinenumberForOperation() throws: " << e.what();
		clog << e.what();
		return 0;
	}

	return lineNumber;

}

string Controller::displayAll() {
	DisplayItems *displayItemsCommand = new DisplayItems();
	
	string output="";
	displayItemsCommand->executeAction(vectorStore, output);
	
	return output;

}

void Controller::clearAll() {
	ClearItems *clearItemsCommand = new ClearItems;

	clearItemsCommand->executeAction(vectorStore);

	if(outputFile.clearFile()) {
		setSuccessMessage(clearItemsCommand->getMessage());
	} else {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
}

void Controller::sortAlphabetical() {

	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical();
	sortAlphabeticalCommand->executeAction(vectorStore);

	setInputBoxMessage("");
	setSuccessMessage(sortAlphabeticalCommand->getMessage());
}

void Controller::search(string searchText) {
	ostringstream oss;
	transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);
	for (unsigned int i = 0; i < vectorStore.size(); i++) {
		string currentString = vectorStore[i].event;
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		size_t position = currentString.find(searchText);
		if (position != string::npos) {
			oss << (i + 1) << ". " << vectorStore[i].event << endl << endl;
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
	CopyItem *copyItemCommand = new CopyItem(getLineNumberForOperation());
	copyItemCommand->executeAction(vectorStore);

	string copiedData = copyItemCommand->getCopiedData();
	if(copiedData!="") {
		outputFile.addLine(copiedData);
		setSuccessMessage(copyItemCommand->getMessage());
	}
	setInputBoxMessage("");

}

void Controller::edit() {
	if(_isFirstCommandCall) {
		int lineNumber = getLineNumberForOperation();
		if(lineNumber == 0) {
			setSuccessMessage(ERROR_INVALID_LINE_NUMBER);
			setInputBoxMessage("");
		} else {
			ostringstream oss;

			oss << vectorStore[lineNumber - 1].event;
			oss << "[" << vectorStore[lineNumber-1].eventDate[0];
			oss << "/" << vectorStore[lineNumber-1].eventDate[1];
			oss << ", " << vectorStore[lineNumber-1].eventStartTime[0];
			oss << ":" << vectorStore[lineNumber-1].eventStartTime[1] << "]";

			string lineToCopy = "edit " + oss.str();
			setSuccessMessage("");
			setInputBoxMessage(lineToCopy);
		}
		_isFirstCommandCall = false;
		_lineNumberOperation = lineNumber;
	} else {
		sprintf_s(buffer, SUCCESS_EDITED.c_str(), 
			vectorStore[_lineNumberOperation - 1].event.c_str(), 
			parser->getEvent().c_str());

		Item temp = initializeItem(parser->getEvent(),
			parser->getDay(),
			parser->getMonth(),
			parser->getHour(),
			parser->getMinute(),
			7);

		vectorStore[_lineNumberOperation - 1] = temp;
		rewriteFile();
		_isFirstCommandCall = true;
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

vector<Item> Controller::getVectorStore() {
	return vectorStore;
}

Controller::~Controller(void) {
}
