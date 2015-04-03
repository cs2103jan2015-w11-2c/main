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
	string archiveFileName;
	string inputBankFileName;
	string optionFileName;
	string autoCompleteFileName;
	static FileStorage* theOne;
	FileStorage(void);

public:

	static FileStorage* getInstance();

	void setFileName(string);

	void setFilePath(string);

	string getFileName();

	string getFileLocation();

	string getFullFileName();

	vector<Item> getAllFileData();

	vector<Item> getArchiveData();

	vector<string> getInputBankData();

	vector<string> getAutoCompleteFileData();

	//main text file
	void addLineToFile(Item);

	//archive file
	void addLineToArchive(Item);
	
	//inputBank file
	void addLineToInputBank(string);

	//auto-complete file
	void addLineToAutoCompleteFile(string); 

	//option file
	void addLineToOptions(string);

	void addLine(Item, const string&);

	bool clearFile();
	
	//clear auto-complete file
	bool clearAutoCompleteFile();

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
	void getFileConfigDEBUG();

	//initializes the filename and filepath if not previously set
	void initializeFileConfig();

	//update any filepath or filename changes into config file
	void updateFileConfigDEBUG();

	//returns the directory of the program
	string programFilePath();

	~FileStorage(void);
};

