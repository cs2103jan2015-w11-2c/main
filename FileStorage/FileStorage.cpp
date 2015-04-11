#include "FileStorage.h"

//@author A0115452N
FileStorage::FileStorage(void) {
	_fileConfigFileName = "FileConfigurations.txt";
	_defaultFileName = "MagicMemo Task List.txt";
	_archiveFileName = "Backup.txt";
	_optionFileName = "Options.txt";
	_autoCompleteFileName = "Auto-complete.txt";

	_is12Hr = true;
	_isWide = false;

	if(isFileEmpty(_fileConfigFileName)) {
		initializeFileConfig();
	}

	getFileConfigInfo();
	_fullFileName = getFullFileName();
}

FileStorage*FileStorage::theOne=nullptr;

FileStorage*FileStorage::getInstance() {
	if(theOne==nullptr) {
		theOne = new FileStorage();
		return theOne;
	}
}

void FileStorage::setFileName(string newFileName) {
	_fileName = newFileName;
}

void FileStorage::setFilePath(string newFilePath) {
	_filePath = newFilePath;
}

string FileStorage::getFileName() {
	return _fileName;
}

string FileStorage::getFileLocation() {
	return _filePath;
}

string FileStorage::getFullFileName() {
	if(_filePath == "") {
		return _fileName;
	} else {
		return (_filePath + "\\" + _fileName);
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

vector<Item> FileStorage::getArchiveData() {
	vector<Item> tempVector;
	Parser parse;
	string content;

	ifstream readFile(_archiveFileName.c_str());
	while(getline(readFile, content)) {
		parse.setStringToParse(content);
		parse.extractDateAndTime();
		Item i = parse.getItem();
		tempVector.push_back(i);
	}
	readFile.close();

	return tempVector;
}

vector<string> FileStorage::getAutoCompleteFileData() {
	vector<string> tempVector;
	string content;

	ifstream readFile(_autoCompleteFileName.c_str());
	while(getline(readFile, content)) {
		tempVector.push_back(content);
	}
	readFile.close();

	return tempVector;
}

vector<int> FileStorage::getOptionFileData() {
	if(isFileEmpty(_optionFileName)) {
		updateOptionsFile();
	}

	vector<int> boolVector;
	int content;
	ifstream readFile(_optionFileName.c_str());
	while(readFile >> content) {
		boolVector.push_back(content);    
	}
	readFile.close();

	return boolVector;
}

void FileStorage::addLineToFile(Item item) {
	addLine(item, getFullFileName());
}

void FileStorage::addLineToArchive(Item item) {
	addLine(item, _archiveFileName);
}

void FileStorage::addLine(Item item, const string& fileName) {
	fstream outFile;
	ostringstream out;
	bool setFrom = false;
	bool setTo = false;

	outFile.open(fileName.c_str(), fstream::out | fstream::app);

	out << item.event;

	if(item.eventDate[0] != 0 && item.eventDate[1] != 0 && item.eventDate[2] != 0) {
		if (item.eventEndTime[0] == 0 && item.eventEndTime[1] == 0) {
			out << " by ";
		} else {
			out << " from ";
		}
		out << item.eventDate[0] << "/" << item.eventDate[1] << "/" << item.eventDate[2];
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


void FileStorage::addLineToAutoCompleteFile(string s) {
	fstream outFile;

	outFile.open(_autoCompleteFileName.c_str(), fstream::out | fstream::app);
	outFile << s << endl;
	outFile.close();
}

void FileStorage::saveIs12Hr(bool is12Hr) {
	_is12Hr = is12Hr;
	updateOptionsFile();
}

void FileStorage::saveIsWide(bool isWide) {
	_isWide = isWide;
	updateOptionsFile();
}

void FileStorage::updateOptionsFile() {
	ofstream outFile(_optionFileName.c_str());
	outFile << _is12Hr << endl;
	outFile << _isWide << endl;
	outFile.close();
}

bool FileStorage::clearFile() {
	fstream outFile;
	outFile.open(getFullFileName(), fstream::out | fstream::trunc);
	outFile.close();
	return true;
}

bool FileStorage::clearAutoCompleteFile() {
	fstream outFile;
	outFile.open(_autoCompleteFileName.c_str(), fstream::out | fstream::trunc);
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
    assert(!newFileName.empty());//new FileName cannot be empty or null
	rename(oldFileName.c_str(), getFullFileName().c_str());

	updateFileConfigInfo();

	return true;
}

bool FileStorage::fileExists(const string& fileName) {
	return (ifstream(fileName.c_str()));
}

bool FileStorage::changeFileLocation(string newFilePath) {
	if(!directoryExists(newFilePath)) {
		return false;
	}

	string newFullFileName = newFilePath + "\\" + _fileName;
	assert(!newFilePath.empty());//newFilePath cannot be empty or null
	if(fileExists(newFullFileName)) {
		return false;
	}

	rename(getFullFileName().c_str(), newFullFileName.c_str());
	setFilePath(newFilePath);

	updateFileConfigInfo();

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


void FileStorage::getFileConfigInfo() {
	ifstream inFile(_fileConfigFileName.c_str());
	getline(inFile, _fileName);
	getline(inFile, _filePath);
	inFile.close();

}

void FileStorage::initializeFileConfig() {
	setFileName(_defaultFileName);
	setFilePath(getProgramFilePath());

	updateFileConfigInfo();
}

void FileStorage::updateFileConfigInfo() {
	ofstream outFile(_fileConfigFileName.c_str());
	outFile << _fileName << endl;
	outFile << _filePath << endl;
	outFile.close();
}

string FileStorage::getProgramFilePath() {
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	string::size_type pos = string( buffer ).find_last_of( "\\/" );
	return string( buffer ).substr( 0, pos);
}

//@author A0115452N
bool FileStorage::restoreFileInfo() {
    if(!changeFileName(_defaultFileName.c_str())) {
		return false;
	}
	if(!changeFileLocation(getProgramFilePath())) {
		return false;
	} 
	return true;
}

FileStorage::~FileStorage(void) {
}
