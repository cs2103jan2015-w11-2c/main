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
		TEST_METHOD(sortAlphabeticalTest) {

		}

		/*	TEST_METHOD(searchTest) {
		Controller controller;
		string inputString = "testing from 23/4/2015 14:0 - 16:0";
		Item item;
		Parser parse;
		parse.setStringToParse(inputString);
		parse.extractDateAndTime();
		item = parse.getItem();


		}*/

		TEST_METHOD(copyTest) {}
		TEST_METHOD(editTest) {}
		TEST_METHOD(renameTest) {}
		TEST_METHOD(moveTest) {}
		TEST_METHOD(undoTest) {}
		TEST_METHOD(redoTest) {}
		TEST_METHOD(gethelpTest) {}

		TEST_METHOD(swapTest) {
			Controller controller;
			/*test for the case when first item has a time but second item only has a date*/
			string inputString1 = "testing from 23/4/2015 14:0 - 16:0";
			string inputString2 = "projec meeting from friday to sunday";
			Item item1;
			Item item2;
			Parser parse;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			string expectedEvent2 = item1.event;
			int expectedEventDay2 = item1.eventDate[0];
			int expectedEventMonth2 = item1.eventDate[1];
			int expectedStartTimeHour2 = item1.eventStartTime[0];
			int expectedStartTimeMin2 = item1.eventStartTime[1];
			string expectedEvent1 = item2.event;
			int expectedEventDay1 = item2.eventDate[0];
			int expectedEventMonth1 = item2.eventDate[1];
			int expectedStartTimeHour1 = item2.eventStartTime[0];
			int expectedStartTimeMin1 = item2.eventStartTime[1];
			controller.swap(item1, item2);

			Assert::AreEqual(expectedEvent2, item2.event);
			Assert::AreEqual(expectedEvent1, item1.event);
			Assert::AreEqual(expectedEventDay2, item2.eventDate[0]);
			Assert::AreEqual(expectedEventDay1, item1.eventDate[0]);
			Assert::AreEqual(expectedEventMonth2, item2.eventDate[1]);
			Assert::AreEqual(expectedEventMonth1, item1.eventDate[1]);
			Assert::AreEqual(expectedStartTimeHour2, item2.eventStartTime[0]);
			Assert::AreEqual(expectedStartTimeHour1, item1.eventStartTime[0]);
			Assert::AreEqual(expectedStartTimeMin2, item2.eventStartTime[1]);
			Assert::AreEqual(expectedStartTimeMin1, item1.eventStartTime[1]);


			/*test for the case when first item has a time,date but second item only has description*/
			inputString1 = "testing from 23/4/2015 14:0 - 16:0";
			inputString2 = "projec meeting";		
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedEvent2 = item1.event;
			expectedEventDay2 = item1.eventDate[0];
			expectedEventMonth2 = item1.eventDate[1];
			expectedStartTimeHour2 = item1.eventStartTime[0];
			expectedStartTimeMin2 = item1.eventStartTime[1];
			expectedEvent1 = item2.event;
			expectedEventDay1 = item2.eventDate[0];
			expectedEventMonth1 = item2.eventDate[1];
			expectedStartTimeHour1 = item2.eventStartTime[0];
			expectedStartTimeMin1 = item2.eventStartTime[1];
			controller.swap(item1, item2);

			Assert::AreEqual(expectedEvent2, item2.event);
			Assert::AreEqual(expectedEvent1, item1.event);
			Assert::AreEqual(expectedEventDay2, item2.eventDate[0]);
			Assert::AreEqual(expectedEventDay1, item1.eventDate[0]);
			Assert::AreEqual(expectedEventMonth2, item2.eventDate[1]);
			Assert::AreEqual(expectedEventMonth1, item1.eventDate[1]);
			Assert::AreEqual(expectedStartTimeHour2, item2.eventStartTime[0]);
			Assert::AreEqual(expectedStartTimeHour1, item1.eventStartTime[0]);
			Assert::AreEqual(expectedStartTimeMin2, item2.eventStartTime[1]);
			Assert::AreEqual(expectedStartTimeMin1, item1.eventStartTime[1]);
		}


		TEST_METHOD(compareEarlierThanTest) {
			Controller controller;
			/*test for the case where two event are not in the same month,event 1 is earlier than 2*/
			string inputString1 = "presentation on 14/4/2015 14:30 ";
			string inputString2 = "study on 3/5/2015 17:00";
			Item item1;
			Item item2;
			Parser parse;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			int expectedResult = -1;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));
		
			/*test for the case where two event are not in the same year,event 1 is earlier than 2*/
			inputString1 = "presentation on 14/4/2015 14:30 ";
			inputString2 = "study on 3/5/2017 17:00";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = -1;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));

			/*test for the case where two event are in the same month,diff day*/
			/*event 1 is earlier than 2*/
			inputString1 = "presentation on 14/4/2015 14:30 ";
			inputString2 = "study on 22/4/2015 17:00";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = -1;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));

			/*test for the case where two event are on the same day,diff start hour*/
			/*event 1 is earlier than 2*/
			inputString1 = "presentation on 11/4/2015 14:30 ";
			inputString2 = "study on 11/4/2015 17:00";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = -1;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));

			/*test for the case where two event are on same start hour,diff start min*/
			/*event 1 is earlier than 2*/
			inputString1 = "presentation on 11/4/2015 14:30 ";
			inputString2 = "study on 11/4/2015 14:54";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = -1;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));

			/*test for the case where two event are at same start time*/
			inputString1 = "presentation on 11/4/2015 14:54";
			inputString2 = "study on 11/4/2015 14:54";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = 0;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));


			/*test for the case where two event are not at same time,but diff duration*/
			inputString1 = "presentation on 11/4/2015 14:30 - 14:50";
			inputString2 = "study on 11/4/2015 14:30 - 17:00";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = 0;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));
		
             /*test for the case where two event areat same time,with same duration*/
			/*event 1 is earlier than 2*/
			inputString1 = "presentation on 11/4/2015 14:30 - 15:50";
			inputString2 = "study on 11/4/2015 14:30 - 15:50";
			item1;
			item2;
			parse.setStringToParse(inputString1);
			parse.extractDateAndTime();
			item1 = parse.getItem();
			parse.setStringToParse(inputString2);
			parse.extractDateAndTime();
			item2 = parse.getItem();

			expectedResult = 0;
			Assert::AreEqual(expectedResult, controller.compareEarlierThan(item1, item2));
}
		TEST_METHOD(chronoSortTest) {}
		TEST_METHOD(addToInputBankTest) {}
		TEST_METHOD(setClockTo12HourTest) {}
		TEST_METHOD(setClockTo24HourTest) {}




	};
}