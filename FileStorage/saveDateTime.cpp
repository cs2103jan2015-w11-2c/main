#include "saveDateTime.h"
#include "Parser.h"
#include "FileStorage.h"
#include "ITEM.cpp"
#include <cassert>


const string FILENAME = "MagicMemo.txt";

const int EVENT_INDEX  = 0;
const int STARTDATE_INDEX = 1;
const int STARTTIME_INDEX = 2;
const int ENDDATE_INDEX   = 3;
const int ENDTIME_INDEX   = 4;

const int DATE_LENGTH  = 8;
const int TIME_LENGTH  = 4;


saveDateTime::saveDateTime(void)
{
}

void saveDateTime::storeDateTime(vector<Item> &content){
	vector<string> tempVector;
	ifstream inFile(getFullFileName());
	string content;
	while (getline(inFile, content)){
	tempVector[1].content=event;
	tempVector[2].content=eventDate[0];
	tempVector[3].content=eventDate[1];
	tempVector[4].content=eventDate[2];
	tempVector[5].startTime=eventStartTime[1];
	tempVector[6].endTime=eventEndTime[1];
	assert(!eventDate.empty());
	tempVector.push_back(event);
	event=parser.getEvent;
	Date[0]=parser.getDay;
	Date[1]=parser.getMonth;
	Date[2]=parser.getHour;
	Date[3]=parser.getMinute;
	inFile.close();}
}

saveDateTime::~saveDateTime(void)
{
}
