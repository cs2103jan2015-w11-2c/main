#include "stdafx.h"

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

		}
		expected = 0;
		Assert::AreEqual(expected, day);
		expected = 0;
		Assert::AreEqual(expected, month);
		expected = 0;
		Assert::AreEqual(expected, year);
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

	TEST_METHOD(extractDateTimeTest3) {
		string inputArray[] = {"12/5/15", "2:30", "-", "4"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 4);
		} catch (const out_of_range& e) {		
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 2;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 30;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 4;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(extractDateTimeTest4) {
		string inputArray[] = {"12/5/15", "3:25", "p", "5"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 4);
		} catch (const out_of_range& e) {		
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 15;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 25;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 20;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 25;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(extractDateTimeTest5) {
		string inputArray[] = {"12/5/15", "3:25", "-", "5", "p"}; 
		Item item;
		DateTimeParser parse;

		try {
			parse.extractDateTime(inputArray, 5);
		} catch (const out_of_range& e) {		
		}
		int expectedDay = 12;
		Assert::AreEqual(expectedDay, parse.getItem().eventDate[0]);
		int expectedMonth = 5;
		Assert::AreEqual(expectedMonth, parse.getItem().eventDate[1]);
		int expectedYear = 2015;
		Assert::AreEqual(expectedYear, parse.getItem().eventDate[2]);

		int expectedStartHour = 3;
		Assert::AreEqual(expectedStartHour, parse.getItem().eventStartTime[0]);
		int expectedStartMinute = 25;
		Assert::AreEqual(expectedStartMinute, parse.getItem().eventStartTime[1]);

		int expectedEndHour = 17;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(updateItemDateTest) {
		string inputString = "12/3/15 6 p - 7";
		Item item;
		DateTimeParser parse;

		try {
			parse.updateItemDateTime(inputString, item);
		} catch (const out_of_range& e) {		
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

		int expectedEndHour = 7;
		Assert::AreEqual(expectedEndHour, parse.getItem().eventEndTime[0]);
		int expectedEndMinute = 0;
		Assert::AreEqual(expectedEndMinute, parse.getItem().eventEndTime[1]);
	}

	TEST_METHOD(mapWeekDayTest1) {
		DateTimeParser parse;
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

		int _day;
		int _month;
		int _year;

		bool expectedResult = true; 
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputMon,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputMonShort,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputTue,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputTueShort1,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputTueShort2,_day,_month,_year));
		//Assert::AreEqual(expectedResult,parse.mapWeekDay(inputWed,_day,_month,_year));
		//Assert::AreEqual(expectedResult,parse.mapWeekDay(inputWedShort,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputThu,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputThuShort1,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputThuShort2,_day,_month,_year));
		
	}

	TEST_METHOD(mapWeekDayTest2) {
		DateTimeParser parse;
		string inputFri = "friday";
		string inputFriShort = "fri";
		string inputSat = "saturday";
		string inputSatShort = "sat";
		string inputSun = "sunday";
		string inputSunShort = "sun";

		int _day;
		int _month;
		int _year;

		bool expectedResult = true; 
		
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputFri,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputFriShort,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputSat,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputSatShort,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputSun,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(inputSunShort,_day,_month,_year));
	}
	//test for invalid input string
	TEST_METHOD(mapWeekDayTest3) {
	DateTimeParser parse;
		string input1 = "today";
		string input2 = "wronginput";
		int _day;
		int _month;
		int _year;
		bool expectedResult = true; 
		Assert::AreEqual(expectedResult,parse.mapWeekDay(input1,_day,_month,_year));
		Assert::AreEqual(expectedResult,parse.mapWeekDay(input2,_day,_month,_year));
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
}