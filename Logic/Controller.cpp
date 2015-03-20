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
	string userCommand = "";
	string commandData = "";
	Item data;
	
	_parser = new Parser(inputText);

	userCommand = _parser->getUserCommand();
	data = _parser->getItem();
	commandData = data.event;

	LOG(INFO) << 	"ITEM Values:";
	LOG(INFO) <<	data.event;
	LOG(INFO) <<	data.eventDate[0];
	LOG(INFO) << 	data.eventDate[1];
	LOG(INFO) << 	data.eventDate[2];
	LOG(INFO) << 	data.eventStartTime[0];
	LOG(INFO) << 	data.eventStartTime[1];
	LOG(INFO) << 	data.eventEndTime[0];
	LOG(INFO) << 	data.eventEndTime[1];

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
		RESULT temp;
		temp.lineNumber = i + 1;
		temp.date = inputVector[i].dateToString();
		temp.time = inputVector[i].timeToString();
		temp.event = inputVector[i].event;
		results.push_back(temp);
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

	chronoSort(_vectorStore);

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

vector<RESULT> Controller::search(string searchText) {
	vector<Item> tempVector = _vectorStore;

	SearchItem *searchItemCommand = new SearchItem(searchText);
	_invoker->executeCommand(tempVector, searchItemCommand, _successMessage);

	chronoSort(tempVector);

	return generateResults(tempVector);
}

vector<RESULT> Controller::copy(Item input) {
	CopyItem *copyItemCommand = new CopyItem(getLineNumberForOperation(), input);
	_invoker->executeCommand(_vectorStore,copyItemCommand, _successMessage);

	if(!rewriteFile()) {
		setSuccessMessage(ERROR_FILE_OPERATION_FAILED);
	}

	chronoSort(_vectorStore);

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

void Controller::swap(Item& item1, Item& item2) {
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

void Controller::chronoSort(vector<Item>& vectorStore) {
	for (unsigned int i = 0; i < (vectorStore.size() - 1); i++) {
		int minIndex = i;
		for (unsigned int j = i + 1; j < vectorStore.size(); j++) {
			if (compareEarlierThan(vectorStore[j], vectorStore[minIndex]) < 0) {
				minIndex=j;
			}
		}
		if(minIndex != i) {
			swap(vectorStore[minIndex],vectorStore[i]);
		}
	}
}


Controller::~Controller(void) {
}
