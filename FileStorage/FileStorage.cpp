#include "FileStorage.h"
//@author A0111951N

FileStorage::FileStorage(void) {
	fileConfigFileName = "fileConfigurations.txt";
	defaultFileName = "MagicMemo Task List.txt";
	archiveFileName = "backup.txt";

	if(isFileEmpty(fileConfigFileName)) {  //if not initialized
		initializeFileConfig();
	}

	getFileConfigDEBUG();
	fullFileName = getFullFileName();
}

//@author YIWEI
FileStorage*FileStorage::theOne=nullptr;

FileStorage*FileStorage::getInstance(){

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

void FileStorage::addLineToFile(Item item) {
	addLine(item, getFullFileName());
}

void FileStorage::addLineToArchive(Item item) {
	addLine(item, archiveFileName);
}

//@author A0111951N
void FileStorage::addLine(Item item, const string& fileName) {
	fstream outFile;
	ostringstream out;
	bool setBracket = false;

	outFile.open(fileName.c_str(), fstream::out | fstream::app);

	out << item.event;

	if(item.eventDate[0] != 0 && item.eventDate[1] != 0 && item.eventDate[2] != 0) {
		out << " from " <<item.eventDate[0] << "/" << item.eventDate[1] << "/" << item.eventDate[2];
		setBracket = true;
	}

	if(item.eventStartTime[0] != 0) {
		if(!setBracket) {
			out << "from ";
		}
		out << " " << item.eventStartTime[0] << ":" << item.eventStartTime[1];
	}
	if(item.eventEndTime[0] !=  0) {
		out << " - " << item.eventEndTime[0] << ":" << item.eventEndTime[1];
	}

	string temp = out.str();
	outFile << temp << endl;
	outFile.close();
}

bool FileStorage::clearFile() {
	fstream outFile;
	outFile.open(getFullFileName(), fstream::out | fstream::trunc);
	outFile.close();
	return true;
}

bool FileStorage::changeFileName(string newFileName) {
	if(fileExists(newFileName)) {
		return false;
	}
	string oldFileName = getFullFileName();
	setFileName(newFileName);
	rename(oldFileName.c_str(), getFullFileName().c_str());
	updateFileConfigDEBUG();
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
	updateFileConfigDEBUG();
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

void FileStorage::getFileConfigDEBUG() {
	ifstream inFile(fileConfigFileName.c_str());
	getline(inFile, fileName);
	getline(inFile, filePath);
	inFile.close();

}

void FileStorage::initializeFileConfig() {
	setFileName(defaultFileName);
	setFilePath(programFilePath());
	updateFileConfigDEBUG();
}

void FileStorage::updateFileConfigDEBUG() {
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

FileStorage::~FileStorage(void) {
}
