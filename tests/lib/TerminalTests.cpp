#include "CppUTest/TestHarness.h"

#include <cstring>
#include <string>

using namespace std;

extern "C" {
#include "CppUTestExt/MockSupport_c.h"
#include <stdint.h>
#include "Terminal.h"
}

TEST_GROUP(TerminalTestGroup) {
	void setup() {
		Terminal_init();
	}

	void teardown() {
		mock_c()->clear();
	}

	void sendCommandWithArgs(const char* cmd, const char* args,
			const char* lineEnd, void* self, bool isCallExpected = true) {
		if (isCallExpected)
			mock_c()->expectOneCall("cmdCallback")->withPointerParameters(
					"self", self)->withStringParameters("args", args);
		string str = string(cmd) + string(args) + string(lineEnd);
		for (size_t i = 0; i < str.size(); i++) {
			Terminal_putCh(str[i]);
		}
	}

	void tickNTimes(int n){
		for (int i=0; i<n; i++){
			Terminal_tick(i*5);
		}
	}
};

extern "C"{
static void cmdCallback(void* self, const char* args){
	mock_c()->actualCall("cmdCallback")->withPointerParameters("self", self)->withStringParameters("args", args);
}
}

TEST(TerminalTestGroup, DoesNotGetStuckWhenNoLineEnding){
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*) cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", "", NULL, false);
	tickNTimes(1);

	mock_c()->checkExpectations();
}

TEST(TerminalTestGroup, RecognizesCrAsLineEnd) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*)cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", "\r", NULL);
	tickNTimes(1);

	mock_c()->checkExpectations();
}

TEST(TerminalTestGroup, RecognizesNlAsLineEnd) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*)cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", "\n", NULL);
	tickNTimes(1);

	mock_c()->checkExpectations();
}

TEST(TerminalTestGroup, RecognizesNlCrAsLineEnd) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*)cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", "\n\r", NULL);
	tickNTimes(3);

	mock_c()->checkExpectations();
}

TEST(TerminalTestGroup, RecognizesCrNlAsLineEnd) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*)cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", "\r\n", NULL);
	tickNTimes(3);

	mock_c()->checkExpectations();
}

TEST(TerminalTestGroup, CanRecognizeCmdWithOutArg) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*)cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", "\n", NULL);
	tickNTimes(1);

	mock_c()->checkExpectations();
}

TEST(TerminalTestGroup, CanRecognizeCmdWithOneArg) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*) cmd.cmdStr, "cmd1 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 1;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd1 ", "1 ", "\n", NULL);
	tickNTimes(1);

	mock_c()->checkExpectations();
}


TEST(TerminalTestGroup, CanRecognizeCmdWithTwoArgs) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*) cmd.cmdStr, "cmd2 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 2;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd2 ", "1 2 ", "\n", NULL);
	tickNTimes(1);

	mock_c()->checkExpectations();
}

