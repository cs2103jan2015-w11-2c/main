#include "Controller.h"
#include "easylogging++.h"

const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";

char Controller::_buffer[1000];

INITIALIZE_EASYLOGGINGPP

Controller::Controller(void) {
	initializeVector();
	_isFirstCommandCall = true;
	_parser = new Parser;
	_outputFile = new FileStorage;
	_invoker = new CommandInvoker;
}

vector<RESULT> Controller::executeCommand(string inputText) {
	_parser = new Parser(inputText);
	_parser->extractUserCommand(inputText);

	string userCommand = _parser->getUserCommand();
	string commandData = _parser->getEvent();

	Item data = _parser->getItem();

	//Item data = _parser->getItem();

	if (userCommand == "display") {
		return displayAll();
	} else if (userCommand == "add") {
		return addData(data);
	} else if (userCommand == "delete") {
		return deleteData();
	} else if (userCommand == "clear") {
		return clearAll();
	} else if (userCommand == "sort") {
		return sortAlphabetical();
	} else if (userCommand == "search") {
		return search(commandData);
	} else if (userCommand == "copy") {
		return copy(data);
	} else if (userCommand == "edit") {
		return edit(data);
	} else if (userCommand == "rename") {
		rename(commandData);
	} else if (userCommand == "move") {
		move(commandData);
	} else if (userCommand == "exit") {
		setSuccessMessage("exit");
	}
	return generateResults(_vectorStore);
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

vector<RESULT> Controller::generateResults(vector<Item> inputVector) {
	vector<RESULT> results;
	for (unsigned int i = 0; i < inputVector.size(); i++) {
		results[i].lineNumber = i + 1;
		results[i].date = inputVector[i].dateToString();
		results[i].time = inputVector[i].timeToString();
		results[i].event = inputVector[i].event;
	}

	return results;
}

bool Controller::rewriteFile() {
	_outputFile->clearFile();
	for (unsigned int i = 0; i < _vectorStore.size(); i++) {
		_outputFile->addLine(_vectorStore[i].event);
	}
	return true;
}

void Controller::commandOptions(string command) {

}

vector<RESULT> Controller::addData(Item item) {
	AddItem *addItemCommand = new AddItem(item);
	_invoker->executeCommand(_vectorStore,addItemCommand, _successMessage);
	
	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	sortChronological(_vectorStore);

	return generateResults(_vectorStore);
}

vector<RESULT> Controller::deleteData() {
	DeleteItem *deleteItemCommand = new DeleteItem(getLineNumberForOperation());
	
	_invoker->executeCommand(_vectorStore, deleteItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}
	
	return generateResults(_vectorStore);
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

vector<RESULT> Controller::displayAll() {
	return generateResults(_vectorStore);
	//return _vectorStore;
}

vector<RESULT> Controller::clearAll() {
	ClearItems *clearItemsCommand = new ClearItems;

	_invoker->executeCommand(_vectorStore,clearItemsCommand, _successMessage);

	if(!_outputFile->clearFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	return generateResults(_vectorStore);
}

vector<RESULT> Controller::sortAlphabetical() {

	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical();
	_invoker->executeCommand(_vectorStore,sortAlphabeticalCommand, _successMessage);

	return generateResults(_vectorStore);
}

void Controller::sortChronological(vector<Item> &inputVector) {

	SortChronological *sortChronologicalCommand = new SortChronological();
	_invoker->executeCommand(inputVector,sortChronologicalCommand, _successMessage);
}

vector<RESULT> Controller::search(string searchText) {
	vector<Item> tempVector = _vectorStore;

	SearchItem *searchItemCommand = new SearchItem(searchText);
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);
	
	sortChronological(tempVector);

	return generateResults(tempVector);
}

vector<RESULT> Controller::copy(Item input) {
	CopyItem *copyItemCommand = new CopyItem(getLineNumberForOperation(), input);
	_invoker->executeCommand(_vectorStore,copyItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	sortChronological(_vectorStore);

	return generateResults(_vectorStore);
}

vector<RESULT> Controller::edit(Item data) {
	EditItem *editItemCommand = new EditItem(getLineNumberForOperation(), data);

	_invoker->executeCommand(_vectorStore, editItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	return generateResults(_vectorStore);
}

void Controller::rename(string newFileName) {
	RenameFile *renameFileCommand = new RenameFile(newFileName);
	_invoker->executeCommand(_outputFile, renameFileCommand, _successMessage);

}

void Controller::move(string newFileLocation) {
	MoveFileLocation *moveFileCommand = new MoveFileLocation;
	_invoker->executeCommand(_outputFile, moveFileCommand, _successMessage);
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
