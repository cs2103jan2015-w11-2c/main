#include "stdafx.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
		string inputString = "testing [23/4 4:0 - 6:0";
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
		string inputString = "meet friends at school[1/4/2015 17:25 - 19:20";
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
		
		string expectedOut8 = "Apr";
		Assert::AreEqual(expectedOut8, testVect[7]);
		
		string expectedOut9 = "2015";
		Assert::AreEqual(expectedOut9, testVect[8]);

		string expectedOut10 = "5:25";
		Assert::AreEqual(expectedOut10, testVect[9]);
		
		string expectedOut11 = "7:20";
		Assert::AreEqual(expectedOut11, testVect[10]);

		
	}
	};

	TEST_CLASS(DateTimeParserTest) {
public:
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

	TEST_METHOD(VerifyItemTimeTest) {
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

	TEST_METHOD(VerifyItemDateTest) {
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

	TEST_METHOD(is12HourTest) {
		DateTimeParser parse;
		int expected;
		int hour = 3;

		parse.is12Hour("pm", hour);

		expected = 15;
		Assert::AreEqual(expected, hour);
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

		int expectedDay = 0;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 0;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 0;
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

	TEST_METHOD(mapWeekDayTest1) {
		DateTimeParser parse;
		string inputThu = "thursday";

		int expectedResult = 4;
		Assert::AreEqual(expectedResult, parse.mapWeekDay(inputThu));

	}
	// test for next Monday(when monday of this week alr passes)
	TEST_METHOD(mapWeekDayTest2) {
		DateTimeParser parse;
		string inputFri = "friday";
		string inputFriShort = "fri";
		string inputSat = "saturday";
		string inputSatShort = "sat";
		string inputSun = "sunday";
		string inputSunShort = "sun";

		string inputMon = "monday";
		string inputMonShort = "mon";
		string inputTue = "tuesday";
		string inputTueShort1 = "tue";
		string inputTueShort2 = "tues";
		string inputWed = "wednesday";
		string inputWedShort = "wed";
		string inputThu = "thursday";
		string inputThuShort1 = "thurs";
		string inputThuShort2 = "thur";

		int expected = 1;
		Assert::AreEqual(expected, parse.mapWeekDay(inputMon));
	}
	/*
	//test for next wednesday(while today is wednesday) and month increases by one
	TEST_METHOD(mapWeekDayTest3) {
	DateTimeParser parse;
	string inputWed = "wednesday";
	string inputWedShort = "wed";

	int _date;
	int _month;
	int _year;

	int expectedDay = 1;
	int expectedMonth = 4;
	int expectedYear = 2015;
	parse.mapWeekDay(inputWed, _date, _month, _year);

	bool expectedResult = true; 
	Assert::AreEqual(expectedDay, _date);
	Assert::AreEqual(expectedMonth, _month);
	Assert::AreEqual(expectedYear, _year);
	Assert::AreEqual(expectedResult,parse.mapWeekDay(inputWed,_date,_month,_year));
	}

	//test for invalid input string	
	TEST_METHOD(mapWeekDayTest4) {
	DateTimeParser parse;
	string input1 = "today";
	string input2 = "wronginput";
	int _day;
	int _month;
	int _year;
	bool expectedResult = false; 
	Assert::AreEqual(expectedResult,parse.mapWeekDay(input1,_day,_month,_year));
	Assert::AreEqual(expectedResult,parse.mapWeekDay(input2,_day,_month,_year));
	}
	*/

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
		int expectedZero = 0;
		Assert::AreEqual(expectedZero,parse.mapMonth(test1));
		Assert::AreEqual(expectedZero,parse.mapMonth(test2));
	}

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

		int expected = 3;
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

		expected = "[10:00 am - 12:00 pm]";
		Assert::AreEqual(expected, test.timeToString());

		test.eventStartTime[0] = 16;
		test.eventStartTime[1] = 30;
		test.eventEndTime[0] = 0;
		test.eventEndTime[1] = 0;
		expected = "[4:30 pm]";
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

		string expectedString = "some event: Wednesday, 25 Mar 2015 [11:10 am - 12:10 pm]";
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

		string expectedString[2] = {"some event: Wednesday, 25 Mar 2015 [11:10 am - 12:10 pm]",
			"some event: Friday, 27 Mar 2015 [3:30 am - 6:30 pm]"};

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