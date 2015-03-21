#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include "Item.h"
#include "Parser.h"

using namespace std;

class FileStorage {
private:
	string fileConfigFileName;
	string defaultFileName;
	string fileName;
	string filePath;
	string fullFileName;

public:
	FileStorage(void);

	void setFileName(string);

	void setFilePath(string);

	string getFileName();

	string getFullFileName();

	vector<string> getAllFileData();

	void addLine(string);

	bool clearFile();

	//returns false if new filename already exists
	bool changeFileName(string newFileName); 

	//returns false if the file path is wrong or
	//the file already exists in the new directory
	bool changeFileLocation(string newFileLocation);

	bool fileExists(const string& fileName);

	bool directoryExists(const string& dirName);

	//returns true if file is empty
	bool isFileEmpty(string);

	//get pre-set filepath and filename from config file
	void getFileConfigInfo();

	//initializes the filename and filepath if not previously set
	void initializeFileConfig();

	//update any filepath or filename changes into config file
	void updateFileConfigInfo();

	//returns the directory of the program
	string programFilePath();

	~FileStorage(void);
};

