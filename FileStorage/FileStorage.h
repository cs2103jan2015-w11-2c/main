#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
using namespace std;

class FileStorage
{
private:
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



	~FileStorage(void);
};

