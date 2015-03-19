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


char Controller::_buffer[1000];

INITIALIZE_EASYLOGGINGPP

Controller::Controller(void) {
	initializeVector();
	_isFirstCommandCall = true;
	_parser = new Parser;
	_outputFile = new FileStorage;
	_invoker = new CommandInvoker;
}

string Controller::executeCommand(string inputText) {
	_parser = new Parser(inputText);

	int month = _parser->getMonth();
	int day = _parser->getDay();
	int hour = _parser->getHour();
	int mins = _parser->getMinute();
	int colour = 7; //temp, until parser can set colours

	string userCommand = _parser->getUserCommand();
	string commandData = _parser->getEvent();

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
	_outputFile->clearFile();
	for (unsigned int i = 0; i < _vectorStore.size(); i++) {
		_outputFile->addLine(_vectorStore[i].event);
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

	_invoker->executeCommand(_vectorStore,addItemCommand);

	_outputFile->addLine(item.event);

	setInputBoxMessage("");
	setSuccessMessage(addItemCommand->getMessage());
}

void Controller::deleteData() {
	DeleteItem *deleteItemCommand = new DeleteItem(getLineNumberForOperation());
	
	_invoker->executeCommand(_vectorStore, deleteItemCommand);

	if(rewriteFile()) {
		setSuccessMessage(deleteItemCommand->getMessage());
	} else {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
	
}

int Controller::getLineNumberForOperation() {
	unsigned int lineNumber = 0;
	try {
		lineNumber = _parser->getLineOpNumber();
		if (lineNumber <= 0 || lineNumber > _vectorStore.size()) {
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
	return "";
	//return _vectorStore;
}

void Controller::clearAll() {
	ClearItems *clearItemsCommand = new ClearItems;

	clearItemsCommand->executeAction(_vectorStore);

	if(_outputFile->clearFile()) {
		setSuccessMessage(clearItemsCommand->getMessage());
	} else {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
}

void Controller::sortAlphabetical() {

	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical();
	sortAlphabeticalCommand->executeAction(_vectorStore);

	setInputBoxMessage("");
	setSuccessMessage(sortAlphabeticalCommand->getMessage());
}

void Controller::search(string searchText) {
	ostringstream oss;
	transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);
	for (unsigned int i = 0; i < _vectorStore.size(); i++) {
		string currentString = _vectorStore[i].event;
		transform(currentString.begin(), currentString.end(), currentString.begin(), ::tolower);
		size_t position = currentString.find(searchText);
		if (position != string::npos) {
			oss << (i + 1) << ". " << _vectorStore[i].event << endl << endl;
		}
	}

	if (oss.str() == "") {
		sprintf_s(_buffer, ERROR_SEARCH_ITEM_NOT_FOUND.c_str(), searchText.c_str());
		setSuccessMessage(_buffer);
	}
	//to be changed to be shown in main outputbox
	setSuccessMessage(oss.str());
	setInputBoxMessage("");
}

void Controller::copy() {
	CopyItem *copyItemCommand = new CopyItem(getLineNumberForOperation());
	copyItemCommand->executeAction(_vectorStore);

	string copiedData = copyItemCommand->getCopiedData();
	if(copiedData!="") {
		_outputFile->addLine(copiedData);
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

			oss << _vectorStore[lineNumber - 1].event;
			oss << "[" << _vectorStore[lineNumber-1].eventDate[0];
			oss << "/" << _vectorStore[lineNumber-1].eventDate[1];
			oss << ", " << _vectorStore[lineNumber-1].eventStartTime[0];
			oss << ":" << _vectorStore[lineNumber-1].eventStartTime[1] << "]";

			string lineToCopy = "edit " + oss.str();
			setSuccessMessage("");
			setInputBoxMessage(lineToCopy);
		}
		_isFirstCommandCall = false;
		_lineNumberOperation = lineNumber;
	} else {
		sprintf_s(_buffer, SUCCESS_EDITED.c_str(), 
			_vectorStore[_lineNumberOperation - 1].event.c_str(), 
			_parser->getEvent().c_str());

		Item temp = initializeItem(_parser->getEvent(),
			_parser->getDay(),
			_parser->getMonth(),
			_parser->getHour(),
			_parser->getMinute(),
			7);

		_vectorStore[_lineNumberOperation - 1] = temp;
		rewriteFile();
		_isFirstCommandCall = true;
		setSuccessMessage(_buffer);
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
		if(_outputFile->changeFileName(newFileName)) {
			sprintf_s(_buffer, SUCCESS_FILENAME_CHANGED.c_str(), _outputFile->getFileName().c_str());
			return _buffer;
		} else {
			return ERROR_FILE_ALREADY_EXISTS;
		}
	}
}

string Controller::move(string newFileLocation) {
	if(_outputFile->changeFileLocation(newFileLocation)) {
		sprintf_s(_buffer, SUCCESS_FILE_LOCATION_CHANGED.c_str(), _outputFile->getFullFileName().c_str());
		return _buffer;
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
	return _vectorStore;
}

Controller::~Controller(void) {
}
