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
		TEST_METHOD(checkIsDeadLineTest1) {
		string input = "travelling from Monday to Friday"; 
		Item item;
		Parser parse;
		Controller control;
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected,control.checkIsDeadline(item));
	}

		TEST_METHOD(checkIsDeadLineTest2) {
		string input = "travelling on 5/5";
		Item item;
		Controller control;
		Parser parse;
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
	    bool isExpected = true;
		Assert::AreEqual(isExpected,control.checkIsDeadline(item));
	}

		TEST_METHOD(checkDateisUnsetTest1) {
		string input = "meeting";
		Controller control;
		Item item;
		Parser parse;
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected,control.checkDateIsUnset(item.eventDate));
	}

		TEST_METHOD(checkDateisUnsetTest2) {
		string input = "meeting on friday";
		Controller control;
		Item item;
		Parser parse;
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = false;
		Assert::AreEqual(isExpected,control.checkDateIsUnset(item.eventDate));
   }


		TEST_METHOD(checkIsClashTest1) {
		string input1 = "dinner on Tuesday 5pm";
		string input2 = "outing on Tuesday 5pm";
		Controller control;
		Item item1, item2;
		Parser parse;
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item1 = parse.getItem();
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item2 = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected, control.checkIsClash(item1, item2));
	}

		TEST_METHOD(checkIsClashTest2) {
		string input1 = "meeting on Tuesday 3pm";
		string input2 = "having dinner on Tuesday 4pm";
		Controller control;
		Item item1, item2;
		Parser parse;
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item1 = parse.getItem();
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item2 = parse.getItem();
		bool isExpected = false;
		Assert::AreEqual(isExpected, control.checkIsClash(item1, item2));
	}

		TEST_METHOD(checkIsExpriedTest1) {
		Controller control;
		Item item;
		Parser parse;
		string input = "exams on 25/4/2014";
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected, control.checkIsExpired(item));
	}

		TEST_METHOD(checkIsExpriedTest2) {
		Controller control;
		Item item;
		Parser parse;
		string input = "having class on 4/4/2015";
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = true;
		Assert::AreEqual(isExpected, control.checkIsExpired(item));
	}

		TEST_METHOD(checkIsExpriedTest3) {
		Controller control;
		Item item;
		Parser parse;
		string input = "having class on 4/5/2015";
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpected = false;
		Assert::AreEqual(isExpected, control.checkIsExpired(item));
	
	}

		TEST_METHOD(compareEarlierThanTest1) {
	    Controller control;
		Parser parse;
		Item item1, item2;

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
	}

		TEST_METHOD(compareEarlierThanTest2) {
	    Controller control;
		Parser parse;
		Item item1, item2;

		string input1 = "event on 5/5/2015 9am";
		string input2 = "stuff on 5/5/2015 9am";
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item1 = parse.getItem();
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item2 = parse.getItem();

		int isExpected = 0;
		Assert::AreEqual(isExpected, control.compareEarlierThan(item1, item2));
	}

		TEST_METHOD(compareEarlierThanTest3) {
	    Controller control;
		Parser parse;
		Item item1,item2;

		string input1 = "event on 6/5 9am";
		string input2 = "stuff on 5/5 11am";
		parse.setStringToParse(input1);
		parse.extractDateAndTime();
		item1 = parse.getItem();
		parse.setStringToParse(input2);
		parse.extractDateAndTime();
		item2 = parse.getItem();

		int isExpected = 1;
		Assert::AreEqual(isExpected, control.compareEarlierThan(item1, item2));
	}

		TEST_METHOD(getTimePosTest) {
		Parser parse;
		Controller control;
		Item item;
		string input = "testing on 8/4/2015 8:30pm";
		parse.setStringToParse(input);
		parse.extractDateAndTime();
		item = parse.getItem();
		long expected = 1; 
        Assert::AreEqual(expected,control.getTimePos(item.eventDate,item.eventStartTime));
	} 
	};
}