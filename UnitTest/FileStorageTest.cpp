#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{	  TEST_CLASS(FileStorageTest)	{
public:		
      TEST_METHOD(getFileNameTest) {
	  FileStorage storage;
	  string expected;
	  
	  expected="MagicMemo Task List.txt";
	  Assert::AreEqual(expected, storage.getFileName());
		}


	  TEST_METHOD(addLineTest) {
	  FileStorage storage;
	  Item item;
	  item.event="stuff";
	  item.eventDate[0]=12;
	  item.eventDate[1]=3;
	  item.eventDate[2]=2015;
	  item.eventStartTime[0]=3;
	  item.eventStartTime[1]=0;

	  string content="stuff [12/3 3:0";
	  storage.addLine(item);

	  string expectedEvent="stuff";
	  Assert::AreEqual(expectedEvent, item.event);
	  int expectedDay=12;
	  Assert::AreEqual(expectedDay, item.eventDate[0]);
	  int expectedMonth=3;
	  Assert::AreEqual(expectedMonth, item.eventDate[1]);
	  int expectedYear=2015;
	  Assert::AreEqual(expectedYear, item.eventDate[2]);
	  int expectedStartHour=3;
	  Assert::AreEqual(expectedStartHour, item.eventStartTime[0]);
	  int expectedStartMinute=0;
	  Assert::AreEqual(expectedStartMinute, item.eventStartTime[1]);

	  }
      };
   
    
}