#include "stdafx.h"
#include "Parser.h"
#include "CppUnitTest.h"

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

	TEST_METHOD(LineNumberTest) {
		Parser parse;

		parse.setCommandData("1");
		Assert::AreEqual(true, parse.haveValidLineNumber());
		Assert::AreEqual(1, parse.getLineOpNumber());

		parse.setCommandData("23 ");
		Assert::AreEqual(true, parse.haveValidLineNumber());
		Assert::AreEqual(23, parse.getLineOpNumber());

		parse.setCommandData("23 4");
		Assert::AreEqual(true, parse.haveValidLineNumber());
		Assert::AreEqual(23, parse.getLineOpNumber());

		parse.setCommandData("a 4");
		Assert::AreEqual(false, parse.haveValidLineNumber());
		Assert::AreEqual(0, parse.getLineOpNumber());
	}

	};
}