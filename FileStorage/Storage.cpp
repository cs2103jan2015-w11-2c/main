#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include "Storage.h"
using namespace std;

const char PARAMETER_SEPARATOR = ';';

const string EMPTY_PARAMETER = "";

const int TASKNAME_INDEX  = 0;
const int STARTDATE_INDEX = 1;
const int STARTTIME_INDEX = 2;
const int ENDDATE_INDEX   = 3;
const int ENDTIME_INDEX   = 4;

const int TOKEN_NUMBER = 6;
const int DATE_LENGTH  = 8;
const int TIME_LENGTH  = 4;

Storage::Storage(void)
{      
	_isCorruptedFile  = false;
	_isCorruptedToken = false;
}

void Storage::tokenizeContent(string content, vector<string>& tokenStorage) {
	istringstream iss(content);
	string token;

	while(getline(iss, token, PARAMETER_SEPARATOR)) {
		tokenStorage.push_back(token);
	}

	verifyTokenContent(tokenStorage);
}

//token content will be verified if corrupted due to modification of the text file
void Storage::verifyTokenContent(const vector<string>& tokenStorage) {

	//by right there should be 5 tokens in tokenStorage, including task name, start date, start time, end date, end time 
	//if the number of tokens in tokenStorage is not 5, token is corrupted
	if(tokenStorage.size() != TOKEN_NUMBER)
		_isCorruptedToken = true;

	//if the number of tokens is correct, length and format of tokens are checked
	if(!_isCorruptedToken) {

		if(!tokenStorage[STARTDATE_INDEX].empty()) {
			if(tokenStorage[STARTDATE_INDEX].size() != DATE_LENGTH) {  //non-empty token of start date must have 8 digit, otherwise corrupted
				_isCorruptedToken = true;
			} else {
				if(!isNumber(tokenStorage[STARTDATE_INDEX])) {  //if this token is 8-digit long but it is not a string of integer, then corrupted
					_isCorruptedToken = true;
				}
			}
		}

		if(!tokenStorage[STARTTIME_INDEX].empty()) {
			if(tokenStorage[STARTDATE_INDEX].empty()) {  //non-empty token of start time only exists when there is non-empty token of start time, otherwise corrupted
				_isCorruptedToken = true;
			} else {
				if(tokenStorage[STARTTIME_INDEX].size() != TIME_LENGTH) {  //this token must have 4 digits, otherwise corrupted
					_isCorruptedToken = true;
				} else {
					if(!isNumber(tokenStorage[STARTTIME_INDEX])) {  //if this token is 8-digit long but it is a string of integer, then corrupted
						_isCorruptedToken = true;
					}
				}
			}
		}

		//similar to checking tokenStorage[STARTDATE_INDEX]
		if(!tokenStorage[ENDDATE_INDEX].empty()) {
			if(tokenStorage[ENDDATE_INDEX].size() != DATE_LENGTH) {
				_isCorruptedToken = true;
			} else {
				if(!isNumber(tokenStorage[ENDDATE_INDEX])) {
					_isCorruptedToken = true;
				}
			}
		}

		//similar to checking tokenStorage[STARTTIME_INDEX]
		if(!tokenStorage[ENDTIME_INDEX].empty()) {
			if(tokenStorage[ENDDATE_INDEX].empty()) {
				_isCorruptedToken = true;
			} else {
				if(tokenStorage[ENDTIME_INDEX].size() != TIME_LENGTH) {
					_isCorruptedToken = true;
				} else {
					if(!isNumber(tokenStorage[ENDTIME_INDEX])) {
						_isCorruptedToken = true;
					}
				}
			}
		}
	}
}

//check whether a string purely consists of integers
bool Storage::isNumber(const string& s) {
    string::const_iterator it = s.begin();

    while (it != s.end() && isdigit(*it)) {
		++it;
	}

    return !s.empty() && it == s.end();
}

void Storage::storeDateTime(vector<Item> &content){
	vector<string> tempVector;
	ifstream inFile(getFullFileName());
	string content;
	while (getline(inFile, content)){
	tempVector[1]._event=content;
	tempVector[2]._event=_item.getDay();
	tempVector[3]._event=_item.getMonth();
	tempVector[4]._event=_item.getHour();
	tempVector[5]._event=_item.getMinute();
    tempVector.push_back(event);
	inFile.close();}
}

Storage::~Storage(void)
{
}
