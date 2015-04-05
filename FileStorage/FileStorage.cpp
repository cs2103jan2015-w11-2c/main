#include "FileStorage.h"
//@author A0115452N

FileStorage::FileStorage(void) {
	fileConfigFileName = "fileConfigurations.txt";
	defaultFileName = "MagicMemo Task List.txt";
	archiveFileName = "backup.txt";
	inputBankFileName = "InputBank.txt";
	optionFileName = "Options.txt";
	autoCompleteFileName = "Suggestion.txt";

	if(isFileEmpty(fileConfigFileName)) {  //if not initialized
		initializeFileConfig();
	}

	getFileConfig();
	fullFileName = getFullFileName();
}

FileStorage*FileStorage::theOne=nullptr;

FileStorage*FileStorage::getInstance() {
	if(theOne==nullptr) {
		theOne = new FileStorage();
		return theOne;
	}
}

//@author A0111951N
void FileStorage::setFileName(string newFileName) {
	fileName = newFileName;
}

void FileStorage::setFilePath(string newFilePath) {
	filePath = newFilePath;
}

string FileStorage::getFileName() {
	return fileName;
}

string FileStorage::getFileLocation() {
	return filePath;
}

string FileStorage::getFullFileName() {
	if(filePath == "") {
		return fileName;
	} else {
		return (filePath + "\\" + fileName);
	}
}

vector<Item> FileStorage::getAllFileData() {
	vector<Item> tempVector;
	Parser parse;
	string content;

	ifstream readFile(getFullFileName());
	while(getline(readFile, content)) {
		parse.setStringToParse(content);
		parse.extractDateAndTime();
		Item i = parse.getItem();
		tempVector.push_back(i);
	}
	readFile.close();

	return tempVector;
}

//@author A0115452N
vector<Item> FileStorage::getArchiveData() {
	vector<Item> tempVector;
	Parser parse;
	string content;

	ifstream readFile(archiveFileName.c_str());
	while(getline(readFile, content)) {
		parse.setStringToParse(content);
		parse.extractDateAndTime();
		Item i = parse.getItem();
		tempVector.push_back(i);
	}
	readFile.close();

	return tempVector;
}


vector<string> FileStorage::getInputBankData() {
	vector<string> tempVector;
	string content;

	ifstream readFile(inputBankFileName.c_str());
	while(getline(readFile, content)) {
		tempVector.push_back(content);
    }
	readFile.close();

	return tempVector;
}

vector<string> FileStorage::getAutoCompleteFileData() {
	vector<string> tempVector;
	string content;

	ifstream readFile(autoCompleteFileName.c_str());
	while(getline(readFile, content)) {
		tempVector.push_back(content);
	}
	readFile.close();

	return tempVector;
}

vector<bool> FileStorage::getOptionFileData() {
	vector<bool> boolVector (2);
	string content;
	bool is12 = true;
	bool isWide = true;
	ifstream readFile(optionFileName.c_str());
	while(getline(readFile, content)) {
	    for(int i = 0; i < boolVector.size(); i++) {
		boolVector[0] = is12;
		boolVector[1] = isWide;
	    boolVector.push_back(i);    
	}
	}
	readFile.close();

	return boolVector;
}

void FileStorage::addLineToFile(Item item) {
	addLine(item, getFullFileName());
}

void FileStorage::addLineToArchive(Item item) {
	addLine(item, archiveFileName);
}

void FileStorage::addLine(Item item, const string& fileName) {
	fstream outFile;
	ostringstream out;
	bool setFrom = false;
	bool setTo = false;

	outFile.open(fileName.c_str(), fstream::out | fstream::app);

	out << item.event;

	if(item.eventDate[0] != 0 && item.eventDate[1] != 0 && item.eventDate[2] != 0) {
		out << " from " <<item.eventDate[0] << "/" << item.eventDate[1] << "/" << item.eventDate[2];
		setFrom = true;
	}

	if(item.eventStartTime[0] != 0) {
		if(!setFrom) {
			out << "from ";
		}
		out << " " << item.eventStartTime[0] << ":" << item.eventStartTime[1];
	}

	if(item.eventEndDate[0] != 0 && item.eventEndDate[1] != 0 && item.eventEndDate[2] != 0) {
		out << " to " <<item.eventEndDate[0] << "/" << item.eventEndDate[1] << "/" << item.eventEndDate[2];
		setTo = true;
	}

	if(item.eventEndTime[0] !=  0) {
		if(!setTo) {
			out << " to ";
		}
		out << " " << item.eventEndTime[0] << ":" << item.eventEndTime[1];
	}

	string temp = out.str();
	outFile << temp << endl;
	outFile.close();
}

void FileStorage::addLineToInputBankFile(vector<string> input) {
	fstream outFile;
	vector<string> ::iterator iter;

	outFile.open(inputBankFileName.c_str(), fstream ::out | fstream ::app);
	for (iter = input.begin(); iter != input.end(); iter++) {
    outFile << *iter << endl;
	}
	outFile.close();
}

void FileStorage::addLineToAutoCompleteFile(string s) {
	fstream outFile;

	outFile.open(autoCompleteFileName.c_str(), fstream::out | fstream::app);
	outFile << s << endl;
	outFile.close();
}

void FileStorage::addLineToOptions(string input) {
	fstream outFile;
	ostringstream out;
	bool is12Hr = true;
	bool isWide = true;
	
	outFile.open(optionFileName.c_str(), fstream::out |fstream ::app);
	if (is12Hr) {
	    out << true;
	}
	else {
		out << false;
	}
    if (isWide) {
        out << true;
	}
	else {
		out << false;
	}
	string temp = out.str();
	outFile << temp<< endl;
	outFile.close();
}

//@author A0111951N
bool FileStorage::clearFile() {
	fstream outFile;
	outFile.open(getFullFileName(), fstream::out | fstream::trunc);
	outFile.close();
	return true;
}

//@author A0115452N
bool FileStorage::clearAutoCompleteFile() {
	fstream outFile;
	outFile.open(autoCompleteFileName.c_str(), fstream::out | fstream::trunc);
	outFile.close();
	return true;
}

//@author A0111951N
bool FileStorage::changeFileName(string newFileName) {
	if(fileExists(newFileName)) {
		return false;
	}
	string oldFileName = getFullFileName();
	setFileName(newFileName);
	rename(oldFileName.c_str(), getFullFileName().c_str());
	updateFileConfig();
	return true;
}

bool FileStorage::fileExists(const string& fileName) {
	return (ifstream(fileName.c_str()));
}

bool FileStorage::changeFileLocation(string newFilePath) {
	if(!directoryExists(newFilePath)) {
		return false;
	}

	string newFullFileName = newFilePath + "\\" + fileName;

	if(fileExists(newFullFileName)) {
		return false;
	}

	rename(getFullFileName().c_str(), newFullFileName.c_str());
	setFilePath(newFilePath);
	updateFileConfig();
	return true;
}

bool FileStorage::directoryExists(const string& dirName) {
	DWORD ftyp = GetFileAttributesA(dirName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  // wrong path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // valid directory!

	return false;    // invalid directory!
}

bool FileStorage::isFileEmpty(string file) {
	string x;
	ifstream inFile(file.c_str());
	inFile >> x;
	if(x == "") {
		return true;
	}
	return false;
}

void FileStorage::getFileConfig() {
	ifstream inFile(fileConfigFileName.c_str());
	getline(inFile, fileName);
	getline(inFile, filePath);
	inFile.close();

}

void FileStorage::initializeFileConfig() {
	setFileName(defaultFileName);
	setFilePath(programFilePath());
	updateFileConfig();
}

void FileStorage::updateFileConfig() {
	ofstream outFile(fileConfigFileName.c_str());
	outFile << fileName << endl;
	outFile << filePath << endl;
	outFile.close();
}

string FileStorage::programFilePath() {
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
	return string( buffer ).substr( 0, pos);
}

//@author A0115452N
void FileStorage::restoreFileInfo () {
    rename(defaultFileName.c_str(), getFullFileName().c_str());
    string filePath=programFilePath();
	changeFileLocation(filePath);
}

FileStorage::~FileStorage(void) {
}
