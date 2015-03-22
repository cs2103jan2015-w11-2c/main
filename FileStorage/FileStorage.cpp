#include "FileStorage.h"

FileStorage::FileStorage(void) {
	fileConfigFileName = "fileConfigurations.txt";
	defaultFileName = "MagicMemo Task List.txt";

	if(isFileEmpty(fileConfigFileName)) {  //if not initialized
		initializeFileConfig();
	}

	getFileConfigInfo();
	fullFileName = getFullFileName();
}

void FileStorage::setFileName(string newFileName) {
	fileName = newFileName;
}

void FileStorage::setFilePath(string newFilePath) {
	filePath = newFilePath;
}

string FileStorage::getFileName() {
	return fileName;
}

string FileStorage::getFullFileName() {
	if(filePath == "") {
		return fileName;
	} else {
		return (filePath + "\\" + fileName);
	}
}

vector<string> FileStorage::getAllFileData() {
	vector<string> tempVector;
	ifstream inFile(getFullFileName());
	string content;
	while (getline(inFile, content)) {
		tempVector.push_back(content);
	}
	inFile.close();
	return tempVector;
}

void FileStorage::addLine(Item item) {
	fstream outFile;
	ostringstream out;
	outFile.open(getFullFileName(), fstream::out | fstream::app);
	out << "* "; //Placeholder that will be removed by Parser's extractUserCommand
	if(item.eventDate[0]== 0 && item.eventDate[1] == 0 && item.eventDate[2] == 0) {
		out << item.event;
	} else { 
		out << item.event;
		out << " [" <<item.eventDate[0] << "/" << item.eventDate[1] << "/" << item.eventDate[2];
		out << " " << item.eventStartTime[0] << ":" << item.eventStartTime[1];
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

	string newFullFileName = newFilePath + "\\" + fileName;

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
		return false;  //something is wrong with path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
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
	ifstream inFile(fileConfigFileName.c_str());
	getline(inFile, fileName);
	getline(inFile, filePath);
	inFile.close();

}

void FileStorage::initializeFileConfig() {
	setFileName(defaultFileName);
	setFilePath(programFilePath());
	updateFileConfigInfo();
}

void FileStorage::updateFileConfigInfo() {
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
