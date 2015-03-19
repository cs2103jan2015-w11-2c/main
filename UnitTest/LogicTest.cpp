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
		string expected = "Add";

		parse.extractUserCommand("Add testing");
		Assert::AreEqual(expected, parse.getUserCommand());
		parse.extractUserCommand("Add   testing");
		Assert::AreEqual(expected, parse.getUserCommand());
		parse.extractUserCommand("Add");
		Assert::AreEqual(expected, parse.getUserCommand());

		parse.extractUserCommand("  ");
		expected = "";
		Assert::AreEqual(expected, parse.getUserCommand());
	}

	TEST_METHOD(separateDayMonthTest) {
		Parser parse;
		int expected;

		parse.separateDayMonthYear("12/04/15");
		expected = 12;
		Assert::AreEqual(expected, parse.getItem().eventDate[0]);
		expected = 04;
		Assert::AreEqual(expected, parse.getItem().eventDate[1]);
		expected = 15;
		Assert::AreEqual(expected, parse.getItem().eventDate[2]);

		parse.separateDayMonthYear("12_04");
		expected = 12;
		Assert::AreEqual(expected, parse.getItem().eventDate[0]);
		expected = 04;
		Assert::AreEqual(expected, parse.getItem().eventDate[1]);
		expected = 0;
		Assert::AreEqual(expected, parse.getItem().eventDate[2]);

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
	};

	TEST_CLASS(ITEMTest) {
public:

	TEST_METHOD(tooStringTest) {
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
		test.eventEndTime[0] = -1;
		test.eventEndTime[1] = 0;
		expected = "[4:30 pm]";
		Assert::AreEqual(expected, test.timeToString());

	}


	};
}