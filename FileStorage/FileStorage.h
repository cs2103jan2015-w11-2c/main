#pragma once
//@author A0115452N
#include <assert.h>
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
	string _fileConfigFileName;
	string _defaultFileName;
	string _fileName;
	string _filePath;
	string _fullFileName;
	string _archiveFileName;
	string _optionFileName;
	string _autoCompleteFileName;
	bool _is12Hr;
	bool _isWide;
	bool _isNotificationsOn;
	int _notifyMin;
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

	//returns a vector of auto-complete bank data
	vector<string> getAutoCompleteFileData();

	//returns an options vector:
	//1st element - is12Hr : 1 if true, 0 otherwise;
	//2nd element - isWide: 1 if true, 0 otherwise;
	//3rd element - isNotificationsOn
	//4th elemment - time for notifications
	vector<int> getOptionFileData();

	//main text file
	void addLineToFile(Item);

	//archive file
	void addLineToArchive(Item);
	
	//general method
	void addLine(Item, const string&);

	//auto-complete file
	void addLineToAutoCompleteFile(string); 

	void saveIs12Hr(bool);

	void saveIsWide(bool);

	void saveNotifications(bool, int);

	//update options file
	void updateOptionsFile();

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
	void getFileConfigInfo();

	//initializes the filename and filepath if not previously set
	void initializeFileConfig();

	//update any filepath or filename changes into config file
	void updateFileConfigInfo();

	//returns the directory of the program
	string getProgramFilePath();

	//restore the default filename and filepath
	bool  restoreFileInfo();

	~FileStorage(void);
};

