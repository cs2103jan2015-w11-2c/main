#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//parser.cpp test
namespace UnitTest {
	TEST_CLASS(ParserTest) {
public:
	TEST_METHOD(RemoveSpacePaddingTest) {
		Parser parse;

		string expected = "Testing1";
		Assert::AreEqual(expected, parse.removeSpacePadding("   Testing1   "));

		expected = "Testing2";
		Assert::AreEqual(expected, parse.removeSpacePadding("  Testing2"));

		expected = "Testing3";
		Assert::AreEqual(expected, parse.removeSpacePadding("Testing3"));

		expected = "";
		Assert::AreEqual(expected, parse.removeSpacePadding("  "));
	}

	TEST_METHOD(ExtractUserCommandTest) {
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

	TEST_METHOD(getFragmentedEventTest){
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

	TEST_METHOD(isDateKeywordTest) {
		Parser parse;

		bool isExpectedBool = true;	
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("tuesday"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("mon"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("on"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("from"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("-"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("today"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("tomorrow"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("next"));
		Assert::AreEqual(isExpectedBool,parse.isDateKeyword("pm"));

	}

	TEST_METHOD(checkIsFloatingTest) {
		string inputString1 = "meet friends on 1/4/2015 17:25 - 19:20";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString1);
		parse.extractDateAndTime();
		item = parse.getItem();
		bool isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString2 = "meet friends on 1/4 17:25 - 19:20";
		parse.setStringToParse(inputString2);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString3 = "meet friends on 5/4 17:25";
		parse.setStringToParse(inputString3);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString4 = "meet friends on 1/4";
		parse.setStringToParse(inputString4);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString5 = "meet friends on saturday 19";
		parse.setStringToParse(inputString5);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = false;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString6 = "meet friends on /4 17:25";
		parse.setStringToParse(inputString6);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = true;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString7 = "meet friends";
		parse.setStringToParse(inputString7);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = true;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

		string inputString8 = "meet friends 17:25 - 19";
		parse.setStringToParse(inputString8);
		parse.extractDateAndTime();
		item = parse.getItem();
		isExpectedFloating = true;
		Assert::AreEqual(isExpectedFloating,parse.checkIsFloating(item));

	}

	TEST_METHOD(clearStartAndEndDateTest){
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

	TEST_METHOD(extractSearchQueryTest){
		//to be entered later
	}

	};

	TEST_CLASS(DateTimeParserTest) {
public:
	/* boundary test for end time less than start time */
	TEST_METHOD(updateItemDateTest) {
		string inputString = "12/3/15 6 p - 5:59 p";
		Item item;
		DateTimeParser parse;

		try {
			parse.updateItemDateTime(inputString, item);
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

		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	/*TEST_METHOD(resetDateTimeTest) {
	DateTimeParser parse;
	Item _item;
	DateTime _dateTime;
	parse.resetDateTime();
	int expectedDay = 0;
	int expectedMonth = 0;
	int expectedYear = 0;
	int expectedEndDay = 0;
	int expectedEndMonth = 0;
	int expectedEndYear = 0;
	int expectedStartHour = 0;
	int expectedStartMinute = 0;
	int expectedEndHour = 0;
	int expectedEndMinute = 0;
	Assert::AreEqual(expectedDay, parse._day);


	}*/

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

		Assert::AreEqual(expectedEventDate1, item.eventDate[1]);
		Assert::AreEqual(expectedEventDate2, item.eventDate[2]);
		Assert::AreEqual(expectedEventEndDate0, item.eventEndDate[0]);
		Assert::AreEqual(expectedEventEndDate1, item.eventEndDate[1]);
		Assert::AreEqual(expectedEventEndDate2, item.eventEndDate[2]);
		Assert::AreEqual(expectedEventStartTime0, item.eventStartTime[0]);
		Assert::AreEqual(expectedEventStartTime1, item.eventStartTime[1]);
		Assert::AreEqual(expectedEventEndTime0,	item.eventEndTime[0]);
		Assert::AreEqual(expectedEventEndTime1,	item.eventEndTime[1]);

	}

	/*TEST_METHOD(updateItemFieldsTest) {
	
	
	
	
	
	
	
	}*/

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

		Assert::AreEqual(expectedEventDate1,parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate2,parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate3,parse.getItem().eventDate[2]);

	}

	TEST_METHOD(findDateDelimetersTest) {
		DateTimeParser parse;
		string input = "19/4/2015";
		size_t expectedPos = 2;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));

		input = "19/4 19:00 - 20:00";
		expectedPos = 2;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));

		input = "17:00 - 20";
		expectedPos = string::npos;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));

		input = "tuesday";
		expectedPos = string::npos;
		Assert::AreEqual(expectedPos, parse.findDateDelimiters(input));
	}

	TEST_METHOD(calculateDateTimeTest) {
		/*test fot the case when the input contains date and start and end time of event*/
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
		int expectedEndHour = 24;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);

		/*test for the boundary case when the input dateTimestring is empty*/
		input = ""; 
		parse.calculateDateTime(input);
		expectedDate = 5;
		Assert::AreEqual(expectedDate, parse.getItem().eventDate[0]);
		expectedMonth = 4;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);
	}

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

		try {
			parse.extractDateTime(inputArray, 1);
		} catch (const out_of_range& e) {
			e;
		}

		int expectedDay = 2;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 4;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 2;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 30;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
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
		int expectedDay = 10;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 4;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedEndDay = 15;
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

		int expectedEndDay = 24;
		Assert::AreEqual(expectedEndDay, parse.getItem().eventEndDate[0]);
		int expectedEndMonth = 3;
		Assert::AreEqual(expectedEndMonth, parse.getItem().eventEndDate[1]);
		int expectedEndYear = 2015;
		Assert::AreEqual(expectedEndYear, parse.getItem().eventEndDate[2]);

		int expectedStartHour = 18;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 0;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 0;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}


	// test for next Monday(when monday of this week alr passes)
	TEST_METHOD(mapWeekDayTest) {
		DateTimeParser parse;

		string input = "monday";
		string inputShort = "mon";
		int expected = 1;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));


		input = "tuesday";
		string inputShort1 = "tue";
		string inputShort2 = "tues";
		expected = 2;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort1));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort2));

		input = "wednesday";
		inputShort = "wed";
		expected = 3;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));

		input = "thursday";
		inputShort1 = "thurs";
		inputShort2 = "thur";
		expected = 4;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort1));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort2));

		input = "friday";
		inputShort = "fri";
		expected = 5;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));


		input = "saturday";
		inputShort = "sat";
		expected = 6;
		Assert::AreEqual(expected, parse.mapWeekDay(input));
		Assert::AreEqual(expected, parse.mapWeekDay(inputShort));

		input = "sunday";
		inputShort = "sun";
		expected = 7;
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

	//test for invalid input such as "friday" and "lastMonth"
	TEST_METHOD(mapMonthTest2) {
		DateTimeParser parse;
		string test1= "friday";
		string test2= "lastMonth";
		int expected = -1;
		Assert::AreEqual(expected,parse.mapMonth(test1));
		Assert::AreEqual(expected,parse.mapMonth(test2));
	}


	TEST_METHOD(setDateFromWeekDayTest) {
		DateTimeParser parse;
		int day;
		int month;
		int year;
		/*test for a weekday on the same week*/
		int expectedDay = 5;
		int expectedMonth = 4;
		int expectedYear = 2015;
		parse.setDateFromWeekDay(7, day, month, year);
		Assert::AreEqual(expectedDay, day);
		Assert::AreEqual(expectedMonth, month);
		Assert::AreEqual(expectedYear, year);

		/*test for a weekday on the next week*/
		expectedDay = 9;
		expectedMonth = 4;
		expectedYear = 2015;
		parse.setDateFromWeekDay(4, day, month, year);
		Assert::AreEqual(expectedDay, day);
		Assert::AreEqual(expectedMonth, month);
		Assert::AreEqual(expectedYear, year);
	}


	TEST_METHOD(handleNextWeekDayTest) {
		DateTimeParser parse;
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


	TEST_METHOD(handleDayOverflowTest) {
		DateTimeParser parse;
		/*test for the case where month increment by 1, when the month has 30 days*/
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

		/*test for the case where month increment by 1,when the month has 31 days*/
		day = 35;
		month = 7;
		year =2015;
		expectedDay = 4;
		expectedMonth = 8;
		expectedYear = 2015;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

		/*test for the case when it's Feburary in leap year*/
		day = 30;
		month = 2;
		year = 2016;
		expectedDay = 1;
		expectedMonth = 3;
		expectedYear = 2016;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

		/*test for the case when it's Feburary in non-leap-year*/
		day = 30;
		month = 2;
		year = 2015;
		expectedDay = 2;
		expectedMonth = 3;
		expectedYear = 2015;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

		/*test for the case where year increment by 1*/
		day = 35;
		month = 12;
		year =2015;
		expectedDay = 4;
		expectedMonth = 1;
		expectedYear = 2016;
		parse.handleDayOverflow(day, month, year);
		Assert::AreEqual(expectedDay,day);
		Assert::AreEqual(expectedMonth,month);
		Assert::AreEqual(expectedYear,year);

	}
	TEST_METHOD(handleImplicitNextTest) {
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

		startDay = 31;
		startMonth = 4 ; 
		startYear = 2015;
		endDay = 27;
		endMonth = 4;
		endYear = 2015;

		expectedEndDay = 4;
		expectedEndMonth = 5;
		expectedEndYear = 2015;
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
	/*
	!!!!
	ask
	the 
	input 
	format
	!!!!
	
	TEST_METHOD(isDelimitedDateTest) {
		DateTimeParser parse;
		bool isExpectedDelimited = true;
		string input = "have lunch with Ann on 19/4/2015";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));


		isExpectedDelimited = true;
		input = "have lunch with Ann on 25/5";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));

		isExpectedDelimited = false;
		input = "have lunch with Ann";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));


		isExpectedDelimited = false;
		input = "have lunch with Ann on sunday";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedDelimited, parse.isDelimitedDate(input));
	}*/

	TEST_METHOD(isPossibleTimeTest) {
		DateTimeParser parse;
		bool isExpectedPossible = true;
		string input = "19:00 - 20:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));

		isExpectedPossible = true;
		input = "5p";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));

		isExpectedPossible = false;
		input = "dinner";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));

		isExpectedPossible = false;
		input = "sunday morning 7:00";
		parse.isDelimitedDate(input);
		Assert::AreEqual(isExpectedPossible, parse.isPossibleTime(input));
	}


	TEST_METHOD(is12HourTest) {
		DateTimeParser parse;
		int expected;
		int hour = 3;

		parse.is12Hour("pm", hour);

		expected = 15;
		Assert::AreEqual(expected, hour);
	}


	TEST_METHOD(separateDayMonthYearTest) {
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


		parse.separateDayMonthYear("22_05", day, month, year);
		expected = 22;
		Assert::AreEqual(expected, day);
		expected = 5;
		Assert::AreEqual(expected, month);
		expected = 2015;
		Assert::AreEqual(expected, year);


	}

	TEST_METHOD(separateHourMinuteTest) {
		DateTimeParser parse;
		int expected;
		int hour;
		int minute;

		parse.separateHourMinute("2:30", hour, minute);

		expected = 2;
		Assert::AreEqual(expected, hour);
		expected = 30;
		Assert::AreEqual(expected, minute);

		parse.separateHourMinute("4", hour, minute);

		expected = 4;
		Assert::AreEqual(expected, hour);
		expected = 0;
		Assert::AreEqual(expected, minute);

	}

	TEST_METHOD(VerifyAllDateTimeTest) {
		DateTimeParser parse;
		Item item;
		string input = "25/5/2015 10:00 - 21:45" ;
		parse.updateItemDateTime(input, item);
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

	TEST_METHOD(UpdateItemStartDateTest) {
		DateTimeParser parse;
		Item item;
		string input = "25/12/2015" ;
		parse.updateItemDateTime(input, item);
		parse.updateItemStartDate();
		int expectedEventDate0 = 25; 
		int expectedEventDate1 = 12; 
		int expectedEventDate2 = 2015; 
		Assert::AreEqual(expectedEventDate0, parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[2]);
	/*
	add the case when one or two of day/month/year is missing
	
	*/

		parse.resetItemDateTime();
		parse.updateItemStartDate();
		expectedEventDate0 = 5; 
		expectedEventDate1 = 4; 
		expectedEventDate2 = 2015; 
		Assert::AreEqual(expectedEventDate0, parse.getItem().eventDate[0]);
		Assert::AreEqual(expectedEventDate1, parse.getItem().eventDate[1]);
		Assert::AreEqual(expectedEventDate2, parse.getItem().eventDate[2]);
	}

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

	TEST_METHOD(convertStringToIntegerTest) {
		DateTimeParser parse;
		string input = "1235";
		int expectedOutput = 1235;
        Assert::AreEqual(expectedOutput, parse.convertStringToInteger(input));

	}

	/* how to do
	TEST_METHOD(getUpdateDateFlagTest) {}
	TEST_METHOD(getUpdateTimeFlagTest) {}
	*/
	};


	TEST_CLASS(DateTimeTest) {
public:

	TEST_METHOD(getWeekDayTest) {
		DateTime dateTest;

		string expected = "Thursday";
		Assert::AreEqual(expected, dateTest.getWeekDay(19, 3, 2015));

		expected = "Monday";
		Assert::AreEqual(expected, dateTest.getWeekDay(29, 2, 2016));

	}

	TEST_METHOD(isLeapYearTest) {
		DateTime dateTest;

		bool expected = true;
		Assert::AreEqual(expected, dateTest.isLeapYear(2016));

		expected = false;
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

	TEST_METHOD(toStringTest) {
		Item test;

		test.event = "";
		test.eventDate[0] = 18;
		test.eventDate[1] = 3;
		test.eventDate[2] = 2015;
		test.eventStartTime[0] = 10;
		test.eventStartTime[1] = 0;
		test.eventEndTime[0] = 12;
		test.eventEndTime[1] = 0;
		test.colour = 0;
		test.bold = false;

		string expected = "Wednesday, 18 Mar 2015";
		Assert::AreEqual(expected, test.dateToString());

		expected = "[10-12p]";
		Assert::AreEqual(expected, test.timeToString());

		test.eventStartTime[0] = 16;
		test.eventStartTime[1] = 30;
		test.eventEndTime[0] = 0;
		test.eventEndTime[1] = 0;
		expected = "[4:30p]";
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
		newItem.colour = 7;
		newItem.bold = false;

		string expectedString = "some event: Wednesday, 25 Mar 2015 [11:10-12:10p]";
		string actualString = newItem.toString();

		Assert::AreEqual(expectedString, actualString);
	}

	TEST_METHOD(addItemTest) {
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
		newItem.colour = 7;
		newItem.bold = false;

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
		newItem.colour = 7;
		newItem.bold = false;

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

	//boundary case of zero vs positive non-zero line numbers
	TEST_METHOD(deleteItemTest) {
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
		newItem.colour = 7;
		newItem.bold = false;

		AddItem *addItem = new AddItem(newItem);
		invoker->executeCommand(testVector, addItem, message);

		vector<int> deleteInput;
		deleteInput.push_back(0);

		DeleteItem *deleteItem = new DeleteItem(deleteInput);
		invoker->executeCommand(testVector, deleteItem, message);

		int expectedSize = 1;
		int actualSize = testVector.size();

		Assert::AreEqual(expectedSize, actualSize);

		string expectedMessage = "Invalid line number specified: 0";
		string actualMessage = message;

		Assert::AreEqual(expectedMessage, actualMessage);

		deleteInput.clear();
		deleteInput.push_back(1);

		DeleteItem *deleteItem2 = new DeleteItem(deleteInput);
		invoker->executeCommand(testVector, deleteItem2, message);

		expectedSize = 0;
		actualSize = testVector.size();

		Assert::AreEqual(expectedSize, actualSize);

		expectedMessage = "Deleted line numbers: 1\n";
		actualMessage = message;

		Assert::AreEqual(expectedMessage, actualMessage);

	}
	};
}