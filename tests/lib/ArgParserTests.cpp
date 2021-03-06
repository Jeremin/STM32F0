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

TEST(ArgParserTestGroup, ParseUintCanParseNumber){
	const char* str = "255";
	uint8_t strLen = strlen(str);

	uint8_t value = ArgParser_getUint((char*)str, strLen);
	CHECK_EQUAL(255, value);
}

TEST(ArgParserTestGroup, ParseUintReturnsZeroIfInvalidInput){
	const char* strArray[3] = {"255 ", " ", "a"};

	for (int i=0; i<3; i++){
		uint8_t strLen = strlen(strArray[i]);
		uint8_t value = ArgParser_getUint((char*)strArray[i], strLen);
		CHECK_EQUAL(0, value);
	}
}

TEST(ArgParserTestGroup, ParseUintCanParseBiggestUint32){
	const char* str = "4294967295";
	uint8_t strLen = strlen(str);

	uint32_t value = ArgParser_getUint((char*)str, strLen);
	CHECK_EQUAL(4294967295, value);
}
