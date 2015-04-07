#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(MainControllerTest)
	{
	public:
		
		TEST_METHOD(AddTest) {
		Controller control;
		vector<RESULT> allTasks;
		vector<RESULT> todayTasks;
		control.executeCommand("clear");
		string input = "Add testing from 1/4 3:50 p 1";
		control.executeCommand(input);
		allTasks = control.getOtherResult();
		todayTasks = control.getTodayResult();

		string expectedDate = "Wednesday, 1 Apr 2015";
		string expectedTime = "[3:50p-4:50p]";
		string expectedEvent = "testing";

		Assert::AreEqual(expectedDate, allTasks[0].date);
		Assert::AreEqual(expectedTime, allTasks[0].time);
		Assert::AreEqual(expectedEvent, allTasks[0].event);
	
	}

		//@author A0115452N
		TEST_METHOD(checkIsDeadLineTest) {
		string input1 = "travelling from Monday to Friday"; 
		Item item;
		Parser parse;
		Controller control;
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected,control.checkIsDeadline(item));

		string input2 = "travelling on 5/5"; 
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item = parse.getItem();
	    isExpected = true;
		Assert::AreEqual(isExpected,control.checkIsDeadline(item));
	}

		TEST_METHOD(checkDateisUnsetTest) {
		string input1 = "meeting";
		Controller control;
		Item item;
		Parser parse;
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected,control.checkDateIsUnset(item.eventDate));

		string input2 = "meeting on friday";
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpected = false;
		Assert::AreEqual(isExpected,control.checkDateIsUnset(item.eventDate));
   
	}


		TEST_METHOD(checkIsClashTest) {
		string input1 = "dinner on Tuesday 5pm";
		string input2 = "outing on Tuesday 5pm";
		Controller control;
		Item item1, item2, item3, item4;
		Parser parse;
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item1 = parse.getItem();
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item2 = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected, control.checkIsClash(item1,item2));
		
		string input3 = "meeting on Tuesday 3pm";
		string input4 = "having dinner on Tuesday 4pm";
		parse.setStringToParse(input3);
		parse.extractDateAndTime();
		item3 = parse.getItem();
		parse.setStringToParse(input4);
		parse.extractDateAndTime();
		item4 = parse.getItem();
		isExpected = false;
		Assert::AreEqual(isExpected, control.checkIsClash(item3,item4));
	
	}

		TEST_METHOD(checkIsExpried) {
		Controller control;
		Item item;
		Parser parse;
		string input1 = "exams on 25/4/2014";
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected, control.checkIsExpired(item));
	    
		string input2 = "having class on 4/4/2015";
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpected = true;
		Assert::AreEqual(isExpected, control.checkIsExpired(item));

		string input3 = "having class on 4/5/2015";
		parse.setStringToParse(input3);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpected = false;
		Assert::AreEqual(isExpected, control.checkIsExpired(item));
	
	}

		TEST_METHOD(compareEarlierThanTest) {
	    Controller control;
		Parser parse;
		Item item1, item2;
		Item item3, item4;
		Item item5, item6;

		string input1 = "event on 5/5 9am";
		string input2 = "stuff on 5/5 11am";
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item1 = parse.getItem();
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item2 = parse.getItem();

		int isExpected = -1;
		Assert::AreEqual(isExpected, control.compareEarlierThan(item1, item2));
	 
		string input3 = "event on 5/5/2015 9am";
		string input4 = "stuff on 5/5/2015 9am";
		parse.setStringToParse(input3);
		parse.extractDateAndTime();
		item3 = parse.getItem();
		parse.setStringToParse(input4);
		parse.extractDateAndTime();
		item4 = parse.getItem();

		isExpected = 0;
		Assert::AreEqual(isExpected, control.compareEarlierThan(item3, item4));

		string input5 = "event on 6/5 9am";
		string input6 = "stuff on 5/5 11am";
		parse.setStringToParse(input5);
		parse.extractDateAndTime();
		item5 = parse.getItem();
		parse.setStringToParse(input6);
		parse.extractDateAndTime();
		item6 = parse.getItem();

		isExpected = 1;
		Assert::AreEqual(isExpected, control.compareEarlierThan(item5, item6));
	}
	};
}