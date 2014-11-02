/*
 * ArgParserTests.cpp
 *
 *  Created on: Nov 2, 2014
 *      Author: jonasbachli
 */

#include "CppUTest/TestHarness.h"

#include <cstring>
#include <string>

using namespace std;

extern "C" {
#include "CppUTestExt/MockSupport_c.h"
#include <stdint.h>
#include "ArgParser.h"
}

TEST_GROUP(ArgParserTestGroup) {
	void setup() {

	}

	void teardown() {
		mock_c()->clear();
	}
};

static void myOnArgParsed(char* arg, uint8_t argLength, uint8_t argIndex) {
	mock_c()->actualCall("onArgParsed")->withIntParameters("argLength",
			argLength)->withIntParameters("argIndex", argIndex);
}

TEST(ArgParserTestGroup, CanInit){
	ArgParser argParser;
	const char* str = "255 255 255";
	uint8_t strLen = strlen(str);

	ArgParser_init(&argParser, (char*)str, strLen, NULL);
}

TEST(ArgParserTestGroup, CanParseOneArg){
	ArgParser argParser;
	const char* str = "255";
	uint8_t strLen = strlen(str);

	mock_c()->expectOneCall("onArgParsed")->withIntParameters("argLength",
			3)->withIntParameters("argIndex", 0);

	ArgParser_init(&argParser, (char*)str, strLen, &myOnArgParsed);
	ArgParser_parse(&argParser);

	mock_c()->checkExpectations();
}

TEST(ArgParserTestGroup, CanParseTwoArgs){
	ArgParser argParser;
	const char* str = "255 255";
	uint8_t strLen = strlen(str);

	mock_c()->expectOneCall("onArgParsed")->withIntParameters("argLength", 3)->withIntParameters(
			"argIndex", 0);
	mock_c()->expectOneCall("onArgParsed")->withIntParameters("argLength", 3)->withIntParameters(
				"argIndex", 1);

	ArgParser_init(&argParser, (char*) str, strLen, &myOnArgParsed);
	ArgParser_parse(&argParser);

	mock_c()->checkExpectations();
}
