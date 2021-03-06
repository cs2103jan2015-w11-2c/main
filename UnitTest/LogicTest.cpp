#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//parser.cpp test
namespace UnitTest {
	TEST_CLASS(ParserTest) {
public:
	//@author A0114613U
	TEST_METHOD(setStringToParseTest) {
		Parser parse;
		string input = "Add dinner with friends today";
		parse.setStringToParse(input);
		string expected = "Add dinner with friends today";
		Assert::AreEqual(expected, parse.getItem().event);

		input = " ";
		parse.setStringToParse(input);
		expected = " ";
		Assert::AreEqual(expected, parse.getItem().event);
	}


	TEST_METHOD(getUserCommandTest1) {
		Parser parse;
		parse.setStringToParse("delete task1");
		parse.extractUserCommand();
		string expectedOutput = "delete";
		string output = parse.getUserCommand();
		Assert::AreEqual(expectedOutput, output);

	}

	TEST_METHOD(getUserCommandTest2) {
		Parser parse;
		parse.setStringToParse("display");
		parse.extractUserCommand();
		string expectedOutput = "display";
		string output = parse.getUserCommand();
		Assert::AreEqual(expectedOutput, output);
	}

	TEST_METHOD(getUserCommandTest3) {
		Parser parse;
		parse.setStringToParse("add task1 have lunch on tues");
		parse.extractUserCommand();
		string expectedOutput = "add";
		string output = parse.getUserCommand();
		Assert::AreEqual(expectedOutput, output);}

	TEST_METHOD(getUserCommandTest4) {
		Parser parse;
		parse.setStringToParse("edit task1 have lunch on tues");
		parse.extractUserCommand();
		string expectedOutput = "edit";
		string output = parse.getUserCommand();
		Assert::AreEqual(expectedOutput, output);	
	}

	/*boundary case of invalid input user command*/
	TEST_METHOD(getUserCommandTest5) {
		Parser parse;
		parse.setStringToParse("test invalid input on monday");
		parse.extractUserCommand();
		string expectedOutput = "test";
		string output = parse.getUserCommand();
		Assert::AreEqual(expectedOutput, output);	
	}
	//@author A0111951N
	TEST_METHOD(ExtractUserCommandTest1) {
		Parser parse;
		string expected = "add";

		parse.setStringToParse("Add testing");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());

		parse.setStringToParse("Add   testing");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());

		parse.setStringToParse("Add");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());

		expected = "";
		parse.setStringToParse("    ");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());
	}

	/*boundary case for invalid input command "do"*/
	TEST_METHOD(ExtractUserCommandTest2) {
		Parser parse;
		string expected = "do";

		parse.setStringToParse("Do testing");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());

		parse.setStringToParse("Do   testing");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());

		parse.setStringToParse("DO");
		parse.extractUserCommand();
		Assert::AreEqual(expected, parse.getUserCommand());
	}

	//author A0114613U
	TEST_METHOD(findDateKeyWordTest1) {
		Parser parse;
		string input = "call John from tuesday";
		size_t expectedPos = input.rfind("from");
		string delimiter = "from";
		Assert::AreEqual(expectedPos, parse.findDateKeyWord(input, delimiter));

	}

	TEST_METHOD(findDateKeyWordTest2) {
		Parser parse;
		string input = "call John on tuesday";
		size_t expectedPos = input.rfind("on");
		string delimiter = "on";
		Assert::AreEqual(expectedPos, parse.findDateKeyWord(input, delimiter));

	}

	TEST_METHOD(isCorrectDateDelimiterTest1) {
		Parser parse;
		string input = "meet John on sunday 17";
		size_t pos = input.rfind("on");
		bool isExpectedDel = true;
		Assert::AreEqual(isExpectedDel,parse.isCorrectDateDelimiter(input,pos));

	}
	/*boundary case of invalid delimiter "for"*/
	TEST_METHOD(isCorrectDateDelimiterTest2) {
		Parser parse;
		string input = "meet John for good friday 17:00";
		size_t pos = input.rfind("for");
		bool isExpectedDel = false;
		Assert::AreEqual(isExpectedDel,parse.isCorrectDateDelimiter(input,pos));
	}

	TEST_METHOD(isCorrectDateDelimiterTest3) {
		Parser parse;
		string input = "meet John from wed 17:00";
		size_t pos = input.rfind("from");
		bool isExpectedDel = true;
		Assert::AreEqual(isExpectedDel,parse.isCorrectDateDelimiter(input,pos));
	}


	//@author A0111951N
	TEST_METHOD(extractDateAndTimeTest) {
		string inputString = "testing from 23/4 4:0 - 6:0";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString);
		parse.extractDateAndTime();
		item = parse.getItem();

		string expected = "testing";
		Assert::AreEqual(expected, item.event);

		int expectedDay = 23;
		Assert::AreEqual(expectedDay, item.eventDate[0]);
		int expectedMonth = 4;
		Assert::AreEqual(expectedMonth, item.eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, item.eventDate[2]);

		int expectedStartHour = 4;
		Assert::AreEqual(expectedStartHour, item.eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, item.eventStartTime[1]);
		int expectedEndHour = 6;
		Assert::AreEqual(expectedEndHour, item.eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, item.eventEndTime[1]);
	}

	TEST_METHOD(RemoveSpacePaddingTest1) {
		Parser parse;
		string expected = "Testing1";
		Assert::AreEqual(expected, parse.removeSpacePadding("   Testing1   "));
	}

	TEST_METHOD(RemoveSpacePaddingTest2) {
		Parser parse;
		string expected = "Testing2";
		Assert::AreEqual(expected, parse.removeSpacePadding("  Testing2"));
	}
	TEST_METHOD(RemoveSpacePaddingTest3) {
		Parser parse;
		string expected = "Testing3";
		Assert::AreEqual(expected, parse.removeSpacePadding("Testing3"));
	}

	TEST_METHOD(RemoveSpacePaddingTest4) {
		Parser parse;
		string expected = "";
		Assert::AreEqual(expected, parse.removeSpacePadding("  "));
	}


	//author A0114613U
	TEST_METHOD(convertStringToLowerCaseTest1) {
		Parser parse;
		string input = "ALL CAPITAL LETTERS";
		string expected = "all capital letters";
		Assert::AreEqual(expected, parse.convertStringToLowerCase(input));
	}

	TEST_METHOD(convertStringToLowerCaseTest2) {
		Parser parse;
		string input = "Some Capital lettERS";
		string expected = "some capital letters";
		Assert::AreEqual(expected, parse.convertStringToLowerCase(input));
	}

	TEST_METHOD(convertStringToLowerCaseTest3) {
		Parser parse;
		string input = " ";
		string expected = " ";
		Assert::AreEqual(expected, parse.convertStringToLowerCase(input));
	}

	TEST_METHOD(convertStringToLowerCaseTest5) {
		Parser parse;
		string input = "all small letters";
		string expected = "all small letters";
		Assert::AreEqual(expected, parse.convertStringToLowerCase(input));
	}



	TEST_METHOD(getFragmentedEventTest1){
		string inputString = "meet friends at school from 1/4/2015 17:25 - 19:20";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString);
		parse.extractDateAndTime();
		item = parse.getItem();


		std::vector<string> testVect = parse.getFragmentedEvent();

		string expectedOut1 = "meet friends at school";
		Assert::AreEqual(expectedOut1, testVect[0]);

		string expectedOut2 = "friends at school";
		Assert::AreEqual(expectedOut2, testVect[1]);

		string expectedOut3 = "at school";
		Assert::AreEqual(expectedOut3, testVect[2]);

		string expectedOut4 = "school";
		Assert::AreEqual(expectedOut4, testVect[3]);

		string expectedOut5 = "Wednesday";
		Assert::AreEqual(expectedOut5, testVect[4]);

		string expectedOut6 = "1";
		Assert::AreEqual(expectedOut6, testVect[5]);

		string expectedOut7 = "4";
		Assert::AreEqual(expectedOut7, testVect[6]);

		string expectedOut8 = "April";
		Assert::AreEqual(expectedOut8, testVect[7]);

		string expectedOut9 = "2015";
		Assert::AreEqual(expectedOut9, testVect[8]);

		string expectedOut10 = "5:25";
		Assert::AreEqual(expectedOut10, testVect[9]);

		string expectedOut11 = "7:20";
		Assert::AreEqual(expectedOut11, testVect[10]);

	}

	TEST_METHOD(getFragmentedEventTest2){
		string inputString = "attend lecture on 10/4 16 ";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString);
		parse.extractDateAndTime();
		item = parse.getItem();


		std::vector<string> testVect = parse.getFragmentedEvent();

		string expectedOut1 = "attend lecture";
		Assert::AreEqual(expectedOut1, testVect[0]);

		string expectedOut2 = "lecture";
		Assert::AreEqual(expectedOut2, testVect[1]);

		string expectedOut3 = "Friday";
		Assert::AreEqual(expectedOut3, testVect[2]);

		string expectedOut4 = "10";
		Assert::AreEqual(expectedOut4, testVect[3]);

		string expectedOut5 = "4";
		Assert::AreEqual(expectedOut5, testVect[4]);

		string expectedOut6 = "April";
		Assert::AreEqual(expectedOut6, testVect[5]);

		string expectedOut7 = "2015";
		Assert::AreEqual(expectedOut7, testVect[6]);

		string expectedOut8 = "4";
		Assert::AreEqual(expectedOut8, testVect[7]);

	}


	TEST_METHOD(checkIsFloatingTest1) {
		string inputString1 = "meet friends on 1/4/2015 17:25 - 19:20";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));
	}

	TEST_METHOD(checkIsFloatingTest2) {
		Item item;
		Parser parse;
		string inputString2 = "meet friends on 1/4 17:25 - 19:20";
		parse.setStringToParse(inputString2);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));
	}

	TEST_METHOD(checkIsFloatingTest3) {
		Item item;
		Parser parse;
		string inputString3 = "meet friends on 5/4 17:25";
		parse.setStringToParse(inputString3);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

	}

	TEST_METHOD(checkIsFloatingTest4) {
		Item item;
		Parser parse;
		string inputString4 = "meet friends on 1/4";
		parse.setStringToParse(inputString4);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));
	}

	TEST_METHOD(checkIsFloatingTest5) {
		Item item;
		Parser parse;
		string inputString5 = "meet friends on saturday 19";
		parse.setStringToParse(inputString5);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));
	}

	TEST_METHOD(checkIsFloatingTest6) {
		string inputString1 = "meet friends";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = true;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));
	}
	/*boundary case when the task description contains a date word but it's not a real date*/
	TEST_METHOD(checkIsFloatingTest7) {
		string inputString1 = "meet friends for good friday";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = true;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));
	}

	//@author A0111951N
	TEST_METHOD(clearStartAndEndDateTest1){
		string inputString1 = "meet friends from 1/4/2015 - 5/4/2015";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString1);
		parse.extractDateAndTime();
		item = parse.getItem();
		parse.clearStartAndEndDate(item);
		for (int i = 0; i < 3; i++) {
			Assert::AreEqual(0,item.eventDate[i]);
			Assert::AreEqual(0,item.eventEndDate[i]);
		}
	}

	TEST_METHOD(clearStartAndEndDateTest2){
		Item item;
		Parser parse;
		string inputString2 = "meet friends from sun - 17/4";
		parse.setStringToParse(inputString2);
		parse.extractDateAndTime();
		item = parse.getItem();
		parse.clearStartAndEndDate(item);
		for (int i = 0; i < 3; i++) {
			Assert::AreEqual(0,item.eventDate[i]);
			Assert::AreEqual(0,item.eventEndDate[i]);
		}
	}

	/*
	TEST_METHOD(extractSearchQueryTest){



	}*/

	};

	TEST_CLASS(DateTimeParserTest) {
public:
	//@author A0111951N
	/* boundary test for end time less than start time */
	TEST_METHOD(updateItemDateTest) {
		string inputString = "12/3/15 6 p - 5:59 p";
		Item item;
		DateTimeParser parse;

		try {
			parse.updateItemDateTime(inputString, item, false);
		} catch (const out_of_range& e) {	
			e;
		}

		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 3;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 18;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 19;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(updateItemDateTest2) {
		string inputString = "12/3/15 6 p - 5:59 p";
		Item item;
		DateTimeParser parse;

		try {
			parse.updateItemDateTime(inputString, item, false);
		} catch (const out_of_range& e) {	
			e;
		}

		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 3;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 18;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 19;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}



	//@author A0111951N
	TEST_METHOD(resetItemDateTimeTest) {
		DateTimeParser parse;
		Item item;

		int expectedEventDate1 = 0;
		int expectedEventDate2 = 0;
		int expectedEventEndDate0 = 0;
		int expectedEventEndDate1 = 0;
		int expectedEventEndDate2 = 0;
		int expectedEventStartTime0 = 0;
		int expectedEventStartTime1 = 0;
		int expectedEventEndTime0 = 0;
		int expectedEventEndTime1 = 0;
		parse.resetItemDateTime();

		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[2]);
		Assert::AreEqual(expectedEventEndDate0, parse.getItem().eventEndDate[0]);
		Assert::AreEqual(expectedEventEndDate1, parse.getItem().eventEndDate[1]);
		Assert::AreEqual(expectedEventEndDate2, parse.getItem().eventEndDate[2]);
		Assert::AreEqual(expectedEventStartTime0, parse.getItem().eventStartTime[0]);
		Assert::AreEqual(expectedEventStartTime1, parse.getItem().eventStartTime[1]);
		Assert::AreEqual(expectedEventEndTime0,	parse.getItem().eventEndTime[0]);
		Assert::AreEqual(expectedEventEndTime1,	parse.getItem().eventEndTime[1]);

	}

	//@author A0114613U
	TEST_METHOD(isDateKeywordTest) {
		DateTimeParser parse;

		bool isExpectedBool = false;
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("on"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("from"));

		isExpectedBool = true;
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("tuesday"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("mon"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("-"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("today"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("tomorrow"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("next"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("pm"));

	}

	TEST_METHOD(setDateTest) {
		DateTimeParser parse;
		Item item;
		int day = 9;
		int month = 4;
		int year = 2015;
		parse.setDate(day, month, year);
		int expectedEventDate1 = 9;
		int expectedEventDate2 = 4;
		int expectedEventDate3 = 2015;

		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate3, parse.getItem().eventDate[2]);

		day = 1;
		month = 0;
		year = 0;
		parse.setDate(day, month, year);
		expectedEventDate1 = 1;
		expectedEventDate2 = 0;
		expectedEventDate3 = 0;

		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate3, parse.getItem().eventDate[2]);

	}

	TEST_METHOD(findDateDelimetersTest1) {
		DateTimeParser parse;
		string input = "19/4/2015";
		size_t expectedPos = 2;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));
	}

	TEST_METHOD(findDateDelimetersTest2) {
		DateTimeParser parse;
		string input = "19/4 19:00 - 20:00";
		size_t expectedPos = 2;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));
	}

	TEST_METHOD(findDateDelimetersTest3) {
		DateTimeParser parse;
		string input = "17:00 - 20";
		size_t expectedPos = string::npos;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));
	}

	TEST_METHOD(findDateDelimetersTest4) {
		DateTimeParser parse;
		string input = "tuesday";
		size_t expectedPos = string::npos;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));
	}

	/*boundary case of an invalid empty input*/
	TEST_METHOD(findDateDelimetersTest5) {
		DateTimeParser parse;
		string input = "*()aha$31";
		size_t expectedPos = string::npos;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));
	}

	/*test for the case when the input contains date and start and invalid end time of event*/
	TEST_METHOD(calculateDateTimeTest1) {

		string input = "27/5/2015 12:30 - 24:00"; 
		Item item;
		DateTimeParser parse;
		parse.calculateDateTime(input);
		int expectedDate = 27;
		Assert::AreEqual(expectedDate, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);
		int expectedStartHour = 12;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 30;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);
		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);

	}


	/*test for the case where there is no year and end time*/
	TEST_METHOD(calculateDateTimeTest2) {
		Item item;
		DateTimeParser parse;
		string input = "27/6 12:00"; 
		parse.calculateDateTime(input);
		int expectedDate = 27;
		Assert::AreEqual(expectedDate, parse.getItem().eventDate[0]);
		int expectedMonth = 6;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);
		int expectedStartHour = 12;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);
		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/*test for the case where there is no year and no time*/
	TEST_METHOD(calculateDateTimeTest3) {
		Item item;
		DateTimeParser parse;
		string input = "27/6"; 
		parse.calculateDateTime(input);
		int expectedDate = 27;
		Assert::AreEqual(expectedDate, parse.getItem().eventDate[0]);
		int expectedMonth = 6;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);
		int expectedStartHour = 0;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);
		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/*test for the boundary case when the input dateTimestring is empty*/
	TEST_METHOD(calculateDateTimeTest4) {
		Item item;
		DateTimeParser parse;
		string input = ""; 
		parse.calculateDateTime(input);
		int expectedDate = 15;
		Assert::AreEqual(expectedDate, parse.getItem().eventDate[0]);
		int expectedMonth = 4;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

	}

	//@author A0111951N
	TEST_METHOD(extractDateTimeTest1) {
		string inputArray[] = {"12/3/2015"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 1);
		} catch (const out_of_range& e) {
			e;
		}

		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 3;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 0;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);
		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(extractDateTimeTest2) {
		string inputArray[] = {"2:30"}; 
		Item item;
		DateTimeParser parse;
		DateTime today;

		try {
			parse.extractDateTime(inputArray, 1);
		} catch (const out_of_range& e) {
			e;
		}

		int expectedDay = today.getCurrentDay();
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = today.getCurrentMonth();
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = today.getCurrentYear();
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 2;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 30;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 3;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 30;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);

	}

	/* case of different date delimiters, with and w/o minutes, and non pm time */
	TEST_METHOD(extractDateTimeTest3) {
		string inputArray[] = {"12.5.15", "12:30", "m", "-", "13.5.15", "4"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 6);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedEndDay = 13;
		Assert::AreEqual(expectedEndDay, parse.getItem().eventEndDate[0]);
		int expectedEndMonth = 5;
		Assert::AreEqual(expectedEndMonth, parse.getItem().eventEndDate[1]);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, parse.getItem().eventEndDate[2]);

		int expectedStartHour = 24;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 30;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 4;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/* boundary cases 12 noon to 11 pm */
	TEST_METHOD(extractDateTimeTest4) {
		string inputArray[] = {"12/5/15", "12", "p", "11"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 4);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 12;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 00;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 23;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/* boundary case for full range of time values */
	TEST_METHOD(extractDateTimeTest5) {
		string inputArray[] = {"12/5/15", "1:00", "-", "11:59", "p"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 5);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 1;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 23;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 59;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(extractDateTimeTest6) {
		string inputArray[] = {"12/5/15","1:00", "p", "to", "13/6", "11:59", "p"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 7);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedEndDay = 13;
		Assert::AreEqual(expectedEndDay, parse.getItem().eventEndDate[0]);
		int expectedEndMonth = 6;
		Assert::AreEqual(expectedEndMonth, parse.getItem().eventEndDate[1]);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, parse.getItem().eventEndDate[2]);

		int expectedStartHour = 13;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 23;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 59;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/* case with 'next', weekday, start time, end time, 2 x pm */ 
	TEST_METHOD(extractDateTimeTest7) {
		string inputArray[] = {"next", "friday", "1:00", "p", "to", "wed", "11:59", "p"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 8);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 24;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 4;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedEndDay = 29;
		Assert::AreEqual(expectedEndDay, parse.getItem().eventEndDate[0]);
		int expectedEndMonth = 4;
		Assert::AreEqual(expectedEndMonth, parse.getItem().eventEndDate[1]);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, parse.getItem().eventEndDate[2]);

		int expectedStartHour = 13;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 23;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 59;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/* case with day, month, start time, end time, 1 p */
	TEST_METHOD(extractDateTimeTest8) {
		string inputArray[] = {"24", "mar", "4", "p", "to", "26", "jun", "5:30"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 8);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 24;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 3;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedEndDay = 26;
		Assert::AreEqual(expectedEndDay, parse.getItem().eventEndDate[0]);
		int expectedEndMonth = 6;
		Assert::AreEqual(expectedEndMonth, parse.getItem().eventEndDate[1]);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, parse.getItem().eventEndDate[2]);

		int expectedStartHour = 16;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 5;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 30;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(extractDateTimeTest9) {
		string inputArray[] = {"24", "mar", "6", "p", "-", "6:30"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 6);
		} catch (const out_of_range& e) {	
			e;
		}
		int expectedDay = 24;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 3;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedEndDay = 0;
		Assert::AreEqual(expectedEndDay, parse.getItem().eventEndDate[0]);
		int expectedEndMonth = 0;
		Assert::AreEqual(expectedEndMonth, parse.getItem().eventEndDate[1]);
		int expectedEndYear = 0;
		Assert::AreEqual(expectedEndYear, parse.getItem().eventEndDate[2]);

		int expectedStartHour = 18;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 18;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 30;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	//boundary case for cross-day duration
	TEST_METHOD(addDurationTest) {
		int duration = 1;
		int startHr = 23;
		int startMin = 15; 
		int endHr;
		int endMin;
		int startDay = 15;
		int startMonth = 4;
		int startYear = 2015;
		int endDay;
		int endMonth;
		int endYear;
		Item item;
		DateTimeParser parse;

		parse.addDuration (duration, 
			startHr, 
			startMin,
			endHr, 
			endMin,
			startDay,
			startMonth,
			startYear,
			endDay,
			endMonth,
			endYear);

		int expectedEndDay = 16;
		Assert::AreEqual(expectedEndDay, endDay);
		int expectedEndMonth = 4;
		Assert::AreEqual(expectedEndMonth, endMonth);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, endYear);

		int expectedEndHour = 24;
		Assert::AreEqual(expectedEndHour, endHr);
		int expectedEndMinute = 15;
		Assert::AreEqual(expectedEndMinute, endMin);
	}

	//boundary case for duration > 24
	TEST_METHOD(addDurationTest2) {
		int duration = 25;
		int startHr = 23;
		int startMin = 15; 
		int endHr;
		int endMin;
		int startDay = 15;
		int startMonth = 4;
		int startYear = 2015;
		int endDay;
		int endMonth;
		int endYear;
		Item item;
		DateTimeParser parse;

		parse.addDuration (duration, 
			startHr, 
			startMin,
			endHr, 
			endMin,
			startDay,
			startMonth,
			startYear,
			endDay,
			endMonth,
			endYear);

		int expectedEndDay = 0;
		Assert::AreEqual(expectedEndDay, endDay);
		int expectedEndMonth = 0;
		Assert::AreEqual(expectedEndMonth, endMonth);
		int expectedEndYear = 0;
		Assert::AreEqual(expectedEndYear, endYear);

		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, endHr);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, endMin);
	}

	//case of cross-am/pm duration
	TEST_METHOD(addDurationTest3) {
		int duration = 5;
		int startHr = 9;
		int startMin = 30; 
		int endHr;
		int endMin;
		int startDay = 15;
		int startMonth = 4;
		int startYear = 2015;
		int endDay = 0;
		int endMonth = 0;
		int endYear = 0;
		Item item;
		DateTimeParser parse;

		parse.addDuration (duration, 
			startHr, 
			startMin,
			endHr, 
			endMin,
			startDay,
			startMonth,
			startYear,
			endDay,
			endMonth,
			endYear);

		int expectedEndDay = 0;
		Assert::AreEqual(expectedEndDay, endDay);
		int expectedEndMonth = 0;
		Assert::AreEqual(expectedEndMonth, endMonth);
		int expectedEndYear = 0;
		Assert::AreEqual(expectedEndYear, endYear);

		int expectedEndHour = 14;
		Assert::AreEqual(expectedEndHour, endHr);
		int expectedEndMinute = 30;
		Assert::AreEqual(expectedEndMinute, endMin);
	}

	//boundary case, duration = 24
	TEST_METHOD(addDurationTest4) {
		int duration = 24;
		int startHr = 10;
		int startMin = 30; 
		int endHr;
		int endMin;
		int startDay = 15;
		int startMonth = 4;
		int startYear = 2015;
		int endDay = 0;
		int endMonth = 0;
		int endYear = 0;
		Item item;
		DateTimeParser parse;

		parse.addDuration (duration, 
			startHr, 
			startMin,
			endHr, 
			endMin,
			startDay,
			startMonth,
			startYear,
			endDay,
			endMonth,
			endYear);

		int expectedEndDay = 16;
		Assert::AreEqual(expectedEndDay, endDay);
		int expectedEndMonth = 4;
		Assert::AreEqual(expectedEndMonth, endMonth);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, endYear);

		int expectedEndHour = 10;
		Assert::AreEqual(expectedEndHour, endHr);
		int expectedEndMinute = 30;
		Assert::AreEqual(expectedEndMinute, endMin);
	}

	//@author A0114613U	
	TEST_METHOD(mapWeekDayTest1) {
		DateTimeParser parse;

		string input = "monday";
		string inputShort = "mon";
		int expected = 1;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));
	}

	TEST_METHOD(mapWeekDayTest2) {
		DateTimeParser parse;
		string input = "tuesday";
		string inputShort1 = "tue";
		string inputShort2 = "tues";
		int expected = 2;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort1));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort2));
	}

	TEST_METHOD(mapWeekDayTest3) {
		DateTimeParser parse;
		string input = "wednesday";
		string inputShort = "wed";
		int expected = 3;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));
	}

	TEST_METHOD(mapWeekDayTest4) {
		DateTimeParser parse;
		string input = "thursday";
		string inputShort1 = "thurs";
		string inputShort2 = "thur";
		int expected = 4;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort1));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort2));

	}

	TEST_METHOD(mapWeekDayTest5) {
		DateTimeParser parse;
		string input = "friday";
		string inputShort = "fri";
		int expected = 5;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));

	}


	TEST_METHOD(mapMonthTest){
		DateTimeParser parse;
		string test1= "january";
		string testShort1= "jan";
		string test2= "february";
		string testShort2= "feb";
		string test3= "march";
		string testShort3= "mar";
		string test4= "april";
		string testShort4= "apr";
		string test5= "may";
		string test6= "june";
		string testShort6= "jun";
		string test7= "july";
		string testShort7= "jul";
		string test8= "august";
		string testShort8= "aug";
		string test9= "september";
		string testShort9= "sept";
		string test10= "october";
		string testShort10= "oct";
		string test11= "november";
		string testShort11= "nov";
		string testShort111= "novem";
		string test12= "december";
		string testShort12= "dec";

		int expectedOutput1 = 1;
		int expectedOutput1d = 1;
		int expectedOutput2 = 2;
		int expectedOutput2d = 2;
		int expectedOutput3 = 3;
		int expectedOutput3d = 3;
		int expectedOutput4 = 4;
		int expectedOutput4d = 4;
		int expectedOutput5 = 5;
		int expectedOutput6 = 6;
		int expectedOutput6d = 6;
		int expectedOutput7 = 7;
		int expectedOutput7d = 7;
		int expectedOutput8 = 8;
		int expectedOutput8d = 8;
		int expectedOutput9 = 9;
		int expectedOutput9d = 9;
		int expectedOutput10 = 10;
		int expectedOutput10d = 10;
		int expectedOutput11 = 11;
		int expectedOutput11d = 11;
		int expectedOutput11c = 11;
		int expectedOutput12 = 12;
		int expectedOutput12d = 12;

		Assert::AreEqual(expectedOutput1,parse.mapMonth(test1));
		Assert::AreEqual(expectedOutput1d,parse.mapMonth(testShort1));
		Assert::AreEqual(expectedOutput2,parse.mapMonth(test2));
		Assert::AreEqual(expectedOutput2d,parse.mapMonth(testShort2));
		Assert::AreEqual(expectedOutput3,parse.mapMonth(test3));
		Assert::AreEqual(expectedOutput3d,parse.mapMonth(testShort3));
		Assert::AreEqual(expectedOutput4,parse.mapMonth(test4));
		Assert::AreEqual(expectedOutput4d,parse.mapMonth(testShort4));
		Assert::AreEqual(expectedOutput5,parse.mapMonth(test5));
		Assert::AreEqual(expectedOutput6,parse.mapMonth(test6));
		Assert::AreEqual(expectedOutput6d,parse.mapMonth(testShort6));
		Assert::AreEqual(expectedOutput7,parse.mapMonth(test7));
		Assert::AreEqual(expectedOutput7d,parse.mapMonth(testShort7));
		Assert::AreEqual(expectedOutput8,parse.mapMonth(test8));
		Assert::AreEqual(expectedOutput8d,parse.mapMonth(testShort8));
		Assert::AreEqual(expectedOutput9,parse.mapMonth(test9));
		Assert::AreEqual(expectedOutput9d,parse.mapMonth(testShort9));
		Assert::AreEqual(expectedOutput10,parse.mapMonth(test10));
		Assert::AreEqual(expectedOutput10d,parse.mapMonth(testShort10));
		Assert::AreEqual(expectedOutput11,parse.mapMonth(test11));
		Assert::AreEqual(expectedOutput11d,parse.mapMonth(testShort11));
		Assert::AreEqual(expectedOutput11c,parse.mapMonth(testShort111));
		Assert::AreEqual(expectedOutput12,parse.mapMonth(test12));
		Assert::AreEqual(expectedOutput12d,parse.mapMonth(testShort12));	
	}


	/*test for invalid input such as "friday" and "lastMonth"*/
	TEST_METHOD(mapMonthTest2) {
		DateTimeParser parse;
		string test1= "friday";
		string test2= "lastMonth";
		int expected = -1;
		Assert::AreEqual(expected,parse.mapMonth(test1));
		Assert::AreEqual(expected,parse.mapMonth(test2));
	}

	/*test for a weekday on the same week*/
	TEST_METHOD(setDateFromWeekDayTest1) {
		DateTimeParser parse;
		int day;
		int month;
		int year;
		int expectedDay = 19;
		int expectedMonth = 4;
		int expectedYear = 2015;
		parse.setDateFromWeekDay(7, day, month, year);
		Assert::AreEqual(expectedDay, day);
		Assert::AreEqual(expectedMonth, month);
		Assert::AreEqual(expectedYear, year);

	}

	/*test for a weekday on the next week*/
	TEST_METHOD(setDateFromWeekDayTest2) {
		DateTimeParser parse;
		int day = 9;
		int month = 4;
		int year = 2015;
		int expectedDay = 17;
		int expectedMonth = 4;
		int expectedYear = 2015;
		parse.setDateFromWeekDay(4, day, month, year);
		parse.setDateFromWeekDay(19, day, month, year);
		Assert::AreEqual(expectedDay, day);
		Assert::AreEqual(expectedMonth, month);
		Assert::AreEqual(expectedYear, year);
	}

	/*boundary case of an invalid integer for weekday*/
	TEST_METHOD(setDateFromWeekDayTest3) {
		DateTimeParser parse;
		int day;
		int month;
		int year;
		int expectedDay = 15;
		int expectedMonth = 4;
		int expectedYear = 2015;
		parse.setDateFromWeekDay(-5, day, month, year);
		Assert::AreEqual(expectedDay, day);
		Assert::AreEqual(expectedMonth, month);
		Assert::AreEqual(expectedYear, year);

	}

	TEST_METHOD(handleNextWeekDayTest1) {
		DateTimeParser parse;
		/*test for the normal case */
		int day = 20;
		int month = 4;
		int year = 2015;
		int expectedDay = 27;
		int expectedMonth = 4;
		int expectedYear = 2015;

		parse.handleNextWeekDay(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

	}

	/*test for the case where day is equal to 0*/
	TEST_METHOD(handleNextWeekDayTest2) {
		DateTimeParser parse;
		int day = 0;
		int month = 4;
		int year = 2015;
		int expectedDay = 0;
		int expectedMonth = 4;
		int expectedYear = 2015;

		parse.handleNextWeekDay(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);
	}

	/*test for the case where month increment by 1, when the month has 30 days*/
	TEST_METHOD(handleDayOverflowTest1) {
		DateTimeParser parse;
		int day = 33;
		int month = 4;
		int year =2015;
		int expectedDay = 3;
		int expectedMonth = 5;
		int expectedYear = 2015;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

	}
	/*test for the case where month increment by 1,when the month has 31 days*/
	TEST_METHOD(handleDayOverflowTest2) {
		DateTimeParser parse;
		int day = 35;
		int month = 7;
		int year =2015;
		int expectedDay = 4;
		int expectedMonth = 8;
		int expectedYear = 2015;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

	}


	/*test for the case when it's Feburary in leap year*/
	TEST_METHOD(handleDayOverflowTest3) {
		DateTimeParser parse;
		int day = 30;
		int month = 2;
		int year = 2016;
		int expectedDay = 1;
		int expectedMonth = 3;
		int expectedYear = 2016;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);
	}

	/*test for the case when it's Feburary in non-leap-year*/
	TEST_METHOD(handleDayOverflowTest4) {
		DateTimeParser parse;	
		int day = 30;
		int month = 2;
		int year = 2015;
		int expectedDay = 2;
		int expectedMonth = 3;
		int expectedYear = 2015;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);
	}

	/*test for the case where year increment by 1*/
	TEST_METHOD(handleDayOverflowTest5) {
		DateTimeParser parse;
		int day = 35;
		int 	month = 12;
		int year =2015;
		int 	expectedDay = 4;
		int expectedMonth = 1;
		int expectedYear = 2016;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);
	}

	/*test for the normal case where atart and end date in the same month*/
	TEST_METHOD(handleImplicitNextTest1) {
		DateTimeParser parse;
		int startDay = 18;
		int startMonth = 4 ; 
		int startYear = 2015;
		int endDay = 15;
		int endMonth = 4;
		int endYear = 2015;

		int expectedEndDay = 22;
		int expectedEndMonth = 4;
		int expectedEndYear = 2015;
		parse.handleImplicitNext(startDay, startMonth, startYear, endDay, endMonth, endYear);
		Assert::AreEqual(expectedEndDay, endDay);
		Assert::AreEqual(expectedEndMonth, endMonth);
		Assert::AreEqual(expectedEndYear, endYear);
	}

	/*test for the normal case where atart and end date in the same month*/
	TEST_METHOD(handleImplicitNextTest2) {
		DateTimeParser parse;
		int startDay = 31;
		int	startMonth = 4 ; 
		int startYear = 2015;
		int endDay = 27;
		int endMonth = 4;
		int endYear = 2015;

		int expectedEndDay = 4;
		int expectedEndMonth = 5;
		int expectedEndYear = 2015;
		parse.handleImplicitNext(startDay, startMonth, startYear, endDay, endMonth, endYear);
		Assert::AreEqual(expectedEndDay, endDay);
		Assert::AreEqual(expectedEndMonth, endMonth);
		Assert::AreEqual(expectedEndYear, endYear);

	}

	TEST_METHOD(updateHrDayMonTest) {
		DateTimeParser parse;
		int monthNum = 4;
		int hour = 7;
		int day;
		int month;
		int year;
		int itemHour;
		int expectedHour = 0;
		int expectedDay = 7;
		int expectedMonth = 4;
		int expectedYear = 2015;
		int expectedItemHour = 0;

		parse.updateHrDayMon(monthNum, hour, day, month, year, itemHour);
		Assert::AreEqual(expectedHour, hour);
		Assert::AreEqual(expectedDay, day);
		Assert::AreEqual(expectedMonth, month);
		Assert::AreEqual(expectedYear, year);
		Assert::AreEqual(expectedItemHour, itemHour);

	}


	TEST_METHOD(isDelimitedDateTest1) {
		DateTimeParser parse;
		bool isExpectedDelimited = true;
		string input = "19/4/2015 19:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));

	}

	TEST_METHOD(isDelimitedDateTest2) {
		DateTimeParser parse;
		bool isExpectedDelimited = true;
		string input = "25/5 8:00 - 9:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));

	}

	TEST_METHOD(isDelimitedDateTest3) {
		DateTimeParser parse;
		bool isExpectedDelimited = false;
		string input = "Sunday 11:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));
	}

	TEST_METHOD(isDelimitedDateTest4) {
		DateTimeParser parse;
		bool isExpectedDelimited = false;
		string input = "sunday";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));
	}

	TEST_METHOD(isPossibleTimeTest1) {
		DateTimeParser parse;
		bool isExpectedPossible = true;
		string input = "19:00 - 20:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));
	}

	/*test for the case where it's a time*/
	TEST_METHOD(isPossibleTimeTest2) {
		DateTimeParser parse;	
		bool isExpectedPossible = true;
		string input = "5p";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));
	}


	/*test for the case where it's not a time*/

	TEST_METHOD(isPossibleTimeTest3) {
		DateTimeParser parse;
		bool isExpectedPossible = false;
		string input = "dinner";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));
	}

	/*test for the case where there is a dateDelimiter and is not a possible time*/
	TEST_METHOD(isPossibleTimeTest4) {
		DateTimeParser parse;
		bool isExpectedPossible = false;
		string input = "sunday morning 7:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));
	}

	//@author A0111951N
	TEST_METHOD(is12HourTest) {
		DateTimeParser parse;
		int expected;
		int hour = 3;

		parse.is12Hour("pm", hour);

		expected = 15;
		Assert::AreEqual(expected, hour);
	}


	TEST_METHOD(separateDayMonthYearTest1) {
		DateTimeParser parse;
		int expected;
		int day;
		int month;
		int year;

		parse.separateDayMonthYear("12/04/15", day, month, year);

		expected = 12;
		Assert::AreEqual(expected, day);
		expected = 4;
		Assert::AreEqual(expected, month);
		expected = 15;
		Assert::AreEqual(expected, year);
	}

	TEST_METHOD(separateDayMonthYearTest2) {
		DateTimeParser parse;
		int expected;
		int day;
		int month;
		int year;
		parse.separateDayMonthYear("22_05", day, month, year);
		expected = 22;
		Assert::AreEqual(expected, day);
		expected = 5;
		Assert::AreEqual(expected, month);
		expected = 2015;
		Assert::AreEqual(expected, year);
	}

	TEST_METHOD(separateHourMinuteTest1) {
		DateTimeParser parse;
		int expected;
		int hour;
		int minute;
		parse.separateHourMinute("2:30", hour, minute);
		expected = 2;
		Assert::AreEqual(expected, hour);
		expected = 30;
		Assert::AreEqual(expected, minute);
	}


	TEST_METHOD(separateHourMinuteTest2) {
		DateTimeParser parse;
		int expected;
		int hour;
		int minute;
		parse.separateHourMinute("4", hour, minute);
		expected = 4;
		Assert::AreEqual(expected, hour);
		expected = 0;
		Assert::AreEqual(expected, minute);
	}

	//@author A0114613U
	TEST_METHOD(VerifyAllDateTimeTest1) {
		DateTimeParser parse;
		Item item;
		string input = "25/5/2015 10:00 - 21:45" ;
		parse.updateItemStartDate();
		parse.updateItemDateTime(input, item, false);
		parse.getItem();

		int expectedEventStartTime0 =  10 ;
		int expectedEventStartTime1 = 0; 
		int expectedEventEndTime0 = 21; 
		int expectedEventEndTime1 = 45; 
		int expectedEventDate0 = 25; 
		int expectedEventDate1 = 5; 
		int expectedEventDate2 = 2015; 
		int expectedEventEndDate0 = 0;
		int expectedEventEndDate1 = 0;
		int expectedEventEndDate2 = 0;
		parse.verifyAllDateTime();
		Assert::AreEqual(expectedEventStartTime0, parse.getItem().eventStartTime[0]);
		Assert::AreEqual(expectedEventStartTime1, parse.getItem().eventStartTime[1]);
		Assert::AreEqual(expectedEventEndTime0, parse.getItem().eventEndTime[0]);
		Assert::AreEqual(expectedEventEndTime1, parse.getItem().eventEndTime[1]);
		Assert::AreEqual(expectedEventDate0, parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[2]);
		Assert::AreEqual(expectedEventEndDate0, parse.getItem().eventEndDate[0]);
		Assert::AreEqual(expectedEventEndDate1, parse.getItem().eventEndDate[1]);
		Assert::AreEqual(expectedEventEndDate2, parse.getItem().eventEndDate[2]);
	}

	//@author A0111951N
	TEST_METHOD(VerifyItemDateTest) {
		DateTimeParser parse;
		int expected;
		int day = 20;
		int month = 3;
		int year = 15;

		parse.verifyItemDate(day, month, year);

		expected = 20;
		Assert::AreEqual(expected, day);
		expected = 3;
		Assert::AreEqual(expected, month);
		expected = 2015;
		Assert::AreEqual(expected, year);

		day = 0;
		month = 13;
		year = 15;

		try {
			parse.verifyItemDate(day, month, year);
		} catch (const out_of_range& e) {
			e;
		}
		expected = 0;
		Assert::AreEqual(expected, day);
		expected = 0;
		Assert::AreEqual(expected, month);
		expected = 0;
		Assert::AreEqual(expected, year);
	}

	//@author A0114613U
	/*test for the case where all three day/month/year are present*/
	TEST_METHOD(UpdateItemStartDateTest1) {
		DateTimeParser parse;	
		Item item;
		item.eventDate[0] = 23;
		item.eventDate[1] = 12;
		item.eventDate[2] = 2015;
		parse.updateItemStartDate();
		int expectedEventDate0 = 23; 
		int expectedEventDate1 = 12; 
		int expectedEventDate2 = 2015; 
		Assert::AreEqual(expectedEventDate0, item.eventDate[0]);
		Assert::AreEqual(expectedEventDate1, item.eventDate[1]);
		Assert::AreEqual(expectedEventDate2, item.eventDate[2]);
	}

	/*test for the case where one or two of day/month/year is zero*/
	TEST_METHOD(UpdateItemStartDateTest2) {
		DateTimeParser parse;	
		Item item;
		item.eventDate[0] = 12;
		item.eventDate[1] = 6;
		item.eventDate[2] = 0;
		parse.updateItemStartDate();
		int expectedEventDate0 = 12; 
		int expectedEventDate1 = 6; 
		int expectedEventDate2 = 0; 
		Assert::AreEqual(expectedEventDate0, item.eventDate[0]);
		Assert::AreEqual(expectedEventDate1, item.eventDate[1]);
		Assert::AreEqual(expectedEventDate2, item.eventDate[2]);
	}

	/*boundary case where all of day/month/year are zeros*/
	TEST_METHOD(UpdateItemStartDateTest3) {
		DateTimeParser parse;	
		DateTime datetime;
		Item item;
		item.eventDate[0] = 0;
		item.eventDate[1] = 0;
		item.eventDate[2] = 0;
		parse.resetItemDateTime();
		parse.updateItemStartDate();
		int expectedEventDate0 = datetime.getCurrentDay(); 
		int expectedEventDate1 = datetime.getCurrentMonth(); 
		int expectedEventDate2 = datetime.getCurrentYear(); 
		Assert::AreEqual(expectedEventDate0, parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[2]);
	}

	//@author A0111951N
	TEST_METHOD(VerifyItemTimeTest) {
		DateTimeParser parse;
		int expected;
		int hour = 26;
		int minute = 67;

		try {
			parse.verifyItemTime(hour, minute);
		} catch (const out_of_range& e) {
			e;
		}

		expected = 0;
		Assert::AreEqual(expected, hour);
		expected = 0;
		Assert::AreEqual(expected, minute);
	}

	/* boundary condition, end time 1 minute less than start time */
	TEST_METHOD(VerifyStartEndTimeTest) {
		DateTimeParser parse;
		int expected;

		int startHour = 14;
		int startMinute = 0;
		int endHour = 13;
		int endMinute = 59;

		int startDay = 12;
		int startMonth = 4;
		int startYear = 2015;
		int endDay = 12;
		int endMonth = 4;
		int endYear = 2015;

		try {
			parse.verifyStartEnd(
				startHour,
				startMinute, 
				endHour, 
				endMinute,
				startDay,
				startMonth,
				startYear,
				endDay,
				endMonth,
				endYear);
		} catch (const out_of_range& e) {
			e;
		}

		expected = 0;
		Assert::AreEqual(expected, endHour);
		expected = 0;
		Assert::AreEqual(expected, endMinute);

	}

	//@author A0114613U
	/*test for the case where the input string is a number string*/
	TEST_METHOD(convertStringToIntegerTest1) {
		DateTimeParser parse;
		string input = "1235";
		int expectedOutput = 1235;
		Assert::AreEqual(expectedOutput, parse.convertStringToInteger(input));
	}

	/*test for the case where the input string contains 1 non-integer*/
	TEST_METHOD(convertStringToIntegerTest2) {
		DateTimeParser parse;	
		string input = "12a34";
		int expectedOutput = 12;
		Assert::AreEqual(expectedOutput, parse.convertStringToInteger(input));
	}
	/*test for the case where the input string contains only 1 integer*/
	TEST_METHOD(convertStringToIntegerTest3) {
		DateTimeParser parse;
		string input = "a3f";
		int expectedOutput = 0;
		Assert::AreEqual(expectedOutput, parse.convertStringToInteger(input));
	}

	/*test for the case where the input stringhas no integer*/
	TEST_METHOD(convertStringToIntegerTest4) {
		DateTimeParser parse;		
		string input = "avdd";
		int expectedOutput = 0;
		Assert::AreEqual(expectedOutput, parse.convertStringToInteger(input));
	}


	};

	//@author A0111951N
	TEST_CLASS(DateTimeTest) {
public:

	TEST_METHOD(getWeekDayTest1) {
		DateTime dateTest;
		string expected = "Thursday";
		Assert::AreEqual(expected, dateTest.getWeekDay(19, 3, 2015));
	}

	TEST_METHOD(getWeekDayTest2) {
		DateTime dateTest;	
		string expected = "Monday";
		Assert::AreEqual(expected, dateTest.getWeekDay(29, 2, 2016));
	}

	TEST_METHOD(isLeapYearTest1) {
		DateTime dateTest;
		bool expected = true;
		Assert::AreEqual(expected, dateTest.isLeapYear(2016));
	}

	TEST_METHOD(isLeapYearTest2) {
		DateTime dateTest;
		bool expected = false;
		Assert::AreEqual(expected, dateTest.isLeapYear(2019));
	}

	TEST_METHOD(getCurrentMonthTest) {
		DateTime dateTest;
		int expected = 4;
		Assert::AreEqual(expected, dateTest.getCurrentMonth());
	}

	TEST_METHOD(getCurrentYearTest) {
		DateTime dateTest;
		int expected = 2015;
		Assert::AreEqual(expected, dateTest.getCurrentYear());
	}

	};

	TEST_CLASS(ITEMTest) {
public:

	TEST_METHOD(toStringTest1) {
		Item test;

		test.event = "";
		test.eventDate[0] = 18;
		test.eventDate[1] = 3;
		test.eventDate[2] = 2015;
		test.eventStartTime[0] = 10;
		test.eventStartTime[1] = 0;
		test.eventEndTime[0] = 12;
		test.eventEndTime[1] = 0;
		string expected = "Wednesday, 18 Mar 2015";
		Assert::AreEqual(expected, test.dateToString());
		expected = "[10-12p]";
		Assert::AreEqual(expected, test.timeToString());
	}

	TEST_METHOD(toStringTest2) {
		Item test;
		test.eventStartTime[0] = 16;
		test.eventStartTime[1] = 30;
		test.eventEndTime[0] = 0;
		test.eventEndTime[1] = 0;
		string expected = "[4:30p]";
		Assert::AreEqual(expected, test.timeToString());
	}


	};
	TEST_CLASS(CommandTest) {
public:
	TEST_METHOD(itemToStringTest) {
		Item newItem;

		newItem.event = "some event";
		newItem.eventDate[0] = 25;
		newItem.eventDate[1] = 3;
		newItem.eventDate[2] = 2015;
		newItem.eventStartTime[0] = 11;
		newItem.eventStartTime[1] = 10;
		newItem.eventEndTime[0] = 12;
		newItem.eventEndTime[1] = 10;

		string expectedString = "some event: Wednesday, 25 Mar 2015 [11:10-12:10p]";
		string actualString = newItem.toString();

		Assert::AreEqual(expectedString, actualString);
	}

	TEST_METHOD(addItemTest1) {
		CommandInvoker *invoker = new CommandInvoker;
		vector<Item> testVector;
		Item newItem;
		string message = "";

		newItem.event = "some event";
		newItem.eventDate[0] = 25;
		newItem.eventDate[1] = 3;
		newItem.eventDate[2] = 2015;
		newItem.eventStartTime[0] = 11;
		newItem.eventStartTime[1] = 10;
		newItem.eventEndTime[0] = 12;
		newItem.eventEndTime[1] = 10;

		AddItem *addItem = new AddItem(newItem);
		invoker->executeCommand(testVector, addItem, message);

		newItem.event = "some event";
		newItem.eventDate[0] = 27;
		newItem.eventDate[1] = 3;
		newItem.eventDate[2] = 2015;
		newItem.eventStartTime[0] = 3;
		newItem.eventStartTime[1] = 30;
		newItem.eventEndTime[0] = 18;
		newItem.eventEndTime[1] = 30;

		AddItem *addItem2 = new AddItem(newItem);
		invoker->executeCommand(testVector, addItem2, message);

		int expectedSize = 2;
		int actualSize = testVector.size();

		Assert::AreEqual(expectedSize, actualSize);

		string expectedString[2] = {"some event: Wednesday, 25 Mar 2015 [11:10-12:10p]",
			"some event: Friday, 27 Mar 2015 [3:30-6:30p]"};

		string actualString;

		vector<Item>::iterator iter;

		int i = 0;
		for (iter = testVector.begin(); iter != testVector.end(); iter++, i++) {
			actualString = iter->toString();
			Assert::AreEqual(expectedString[i], actualString);
		}
	}

	};

}