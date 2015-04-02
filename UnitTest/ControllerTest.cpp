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

	};
}