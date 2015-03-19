#include "Controller.h"
#include "easylogging++.h"

const string Controller::SUCCESS_FILENAME_CHANGED = "Filename changed to \"%s\"\n";
const string Controller::SUCCESS_FILE_LOCATION_CHANGED = "File location changed to %s\n";
const string Controller::ERROR_INVALID_LINE_NUMBER = "Invalid line number specified!\n";
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
		edit(data);
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
	_invoker->executeCommand(_vectorStore,addItemCommand, _successMessage);
	
	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
}

void Controller::deleteData() {
	DeleteItem *deleteItemCommand = new DeleteItem(getLineNumberForOperation());
	
	_invoker->executeCommand(_vectorStore, deleteItemCommand, _successMessage);

	if(!rewriteFile()) {
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

	_invoker->executeCommand(_vectorStore,clearItemsCommand, _successMessage);

	if(!_outputFile->clearFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
}

void Controller::sortAlphabetical() {

	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical();
	_invoker->executeCommand(_vectorStore,sortAlphabeticalCommand, _successMessage);

}

void Controller::search(string searchText) {
	vector<Item> tempVector = _vectorStore;

	SearchItem *searchItemCommand = new SearchItem(searchText);
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);

}

void Controller::copy() {
	CopyItem *copyItemCommand = new CopyItem(getLineNumberForOperation());
	_invoker->executeCommand(_vectorStore,copyItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

}

void Controller::edit(Item data) {
	EditItem *editItemCommand = new EditItem(getLineNumberForOperation(), data);
	_invoker->executeCommand(_vectorStore, editItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
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
