#include "Controller.h"
#include "easylogging++.h"

const string Controller::ERROR_FILE_OPERATION_FAILED = "File updating failed!\n";

INITIALIZE_EASYLOGGINGPP

	Controller::Controller(void) {
		_parser = new Parser;
		_outputFile = FileStorage::getInstance();
		_invoker = new CommandInvoker;
		initializeVector();
		_isSearch = false;
		_isWide = true;
}

void Controller::executeCommand(string inputText) {
	string userCommand = "";
	string commandData = "";
	Item data;

	_parser->setStringToParse(inputText);
	_parser->extractUserCommand();
	_parser->extractDateAndTime();

	userCommand = _parser->getUserCommand();
	data = _parser->getItem();
	commandData = data.event;

	LOG(INFO) << 	"ITEM Values:";
	data.logItemValues();


	if(userCommand != "") {
		addToInputBank(commandData);
	}


	if(userCommand == "search") {
		_isSearch = true;
	} else {
		_isSearch = false;
	}

	if (userCommand == "display") {
		displayAll();
	} else if (userCommand == "add") {
		addData(data);
	} else if (userCommand == "delete") {
		deleteData();
	} else if (userCommand == "clear") {
		clearAll();
	} else if (userCommand == "sort") {
		sortAlphabetical();
	} else if (userCommand == "search") {
		search(data);
	} else if (userCommand == "copy") {
		copy(data);
	} else if (userCommand == "edit") {
		edit(data);
	} else if (userCommand == "rename") {
		rename(commandData);
	} else if (userCommand == "move" || userCommand == "save") {
		move(commandData);
	} else if (userCommand == "undo") {
		undo();
	} else if (userCommand == "redo") {
		redo();
	} else if (userCommand == "view" || userCommand == "more") {
		toggleIsWide();
	} else if (userCommand == "exit") {
		setSuccessMessage("exit");
	}
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
	_vectorStore = _outputFile->getAllFileData();
}

void Controller::generateResults(vector<Item> inputVector) {
	vector<RESULT> todayResult;
	vector<RESULT> otherResult;
	DateTime newDateTime;

	for (unsigned int i = 0; i < inputVector.size(); i++) {
		RESULT temp;

		temp.lineNumber = to_string(i + 1) + ".";
		temp.date = inputVector[i].dateToString();
		temp.time = inputVector[i].timeToString();
		temp.event = inputVector[i].event;
		if (inputVector[i].eventDate[0] == newDateTime.getCurrentDay() &&
			inputVector[i].eventDate[1] == newDateTime.getCurrentMonth() &&
			inputVector[i].eventDate[2] == newDateTime.getCurrentYear()) {
				todayResult.push_back(temp);
		} else {
			otherResult.push_back(temp);
		}
	}
	_todayResult = todayResult;
	_otherResult = otherResult;

}

bool Controller::rewriteFile() {
	_outputFile->clearFile();
	for (unsigned int i = 0; i < _vectorStore.size(); i++) {
		_outputFile->addLineToFile(_vectorStore[i]);
	}
	return true;
}

void Controller::commandOptions(string command) {

}

void Controller::addData(Item item) {
	AddItem *addItemCommand = new AddItem(item);
	_invoker->executeCommand(_vectorStore, addItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::deleteData() {
	DeleteItem *deleteItemCommand = new DeleteItem(_parser->getLineOpNumber());

	_invoker->executeCommand(_vectorStore, deleteItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

int Controller::getLineNumberForOperation() {
	unsigned int lineNumber = 0;
	try {
		//lineNumber = _parser->getLineOpNumber();
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

void Controller::displayAll() {
	generateResults(_vectorStore);
	//return _vectorStore;
}

void Controller::clearAll() {
	ClearItems *clearItemsCommand = new ClearItems;

	_invoker->executeCommand(_vectorStore, clearItemsCommand, _successMessage);

	if(!_outputFile->clearFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::sortAlphabetical() {
	SortAlphabetical *sortAlphabeticalCommand = new SortAlphabetical();
	_invoker->executeCommand(_vectorStore, sortAlphabeticalCommand, _successMessage);

	generateResults(_vectorStore);
}

void Controller::search(Item data) {
	vector<Item> tempVector = _vectorStore;

	SearchItem *searchItemCommand = new SearchItem(data, &_otherResult);
	_invoker->disableUndo();
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);
}

bool Controller::isSearch() {
	return _isSearch;
}


void Controller::toggleIsWide() {
	_isWide = !_isWide;
}

bool Controller::isWide() {
	return _isWide;
}

void Controller::copy(Item input) {
	CopyItem *copyItemCommand = new CopyItem(_parser->getLineOpNumber()[0], input);
	_invoker->executeCommand(_vectorStore, copyItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::edit(Item data) {
	int lineNumber = _parser->getLineOpNumber()[0];

	_parser->extractUserCommand();
	Item item = _parser->getItem();

	EditItem *editItemCommand = new EditItem(lineNumber, item);

	_invoker->executeCommand(_vectorStore, editItemCommand, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::rename(string newFileName) {
	RenameFile *renameFileCommand = new RenameFile(newFileName);
	_invoker->executeCommand(_outputFile, renameFileCommand, _successMessage);

}

void Controller::move(string newFileLocation) {
	MoveFileLocation *moveFileCommand = new MoveFileLocation;
	_invoker->executeCommand(_outputFile, moveFileCommand, _successMessage);
}

void Controller::undo() {
	_invoker->undo(_vectorStore, _successMessage);
	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
}

void Controller::redo() {
	_invoker->redo(_vectorStore, _successMessage);

	chronoSort(_vectorStore);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	generateResults(_vectorStore);
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

vector<RESULT> Controller::getTodayResult() {
	return _todayResult;
}

vector<RESULT> Controller::getOtherResult() {
	return _otherResult;
}

void Controller::swap(Item &item1, Item &item2) {
	Item tempItem = item1;
	item1 = item2;
	item2 = tempItem;
}

int Controller::compareEarlierThan(const Item item1, const Item item2) {
	if (item1.eventDate[2] < item2.eventDate[2]) {
		return -1;
	} else if (item1.eventDate[2] == item2.eventDate[2]) {
		if (item1.eventDate[1] < item2.eventDate[1]) {
			return -1;
		} else if (item1.eventDate[1] == item2.eventDate[1]) {
			if (item1.eventDate[0] < item2.eventDate[0]) {
				return -1;
			} else if (item1.eventDate[0] == item2.eventDate[0]) {
				if (item1.eventStartTime[0] < item2.eventStartTime[0]) {
					return -1;
				} else if (item1.eventStartTime[0] == item2.eventStartTime[0]) {
					if (item1.eventStartTime[1] < item2.eventStartTime[1]) {
						return -1;
					} else if (item1.eventStartTime[1] == item2.eventStartTime[1]) {
						return 0;
					}
				}
			}
		}
	}
	return 1;
}

void Controller::chronoSort(vector<Item> &vectorStore) {
	for (int i = 0; i < ((int)vectorStore.size() - 1); i++) {
		int minIndex = i;
		for (unsigned int j = i + 1; j < vectorStore.size(); j++) {
			if (compareEarlierThan(vectorStore[j], vectorStore[minIndex]) < 0) {
				minIndex = j;
			}
		}
		if(minIndex != i) {
			swap(vectorStore[minIndex], vectorStore[i]);
		}
	}
}

void Controller::addToInputBank(const string input) {
	vector<string> fragEvent = _parser->getFragmentedEvent();
	vector<string>::iterator iter1;
	vector<string>::iterator iter2;

	for(iter1 = fragEvent.begin(); iter1 != fragEvent.end(); iter1++) {
		bool isFound = false;
		for(iter2 = _inputBank.begin(); iter2 != _inputBank.end(); iter2++) {
			if (*iter1 == *iter2) {
				isFound = true;
				break;
			}
		}
		if (!isFound) {
			_inputBank.push_back(*iter1);
		}
	}
}

vector<string> Controller::getInputBank() {
	return _inputBank;
}


Controller::~Controller(void) {
}
