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

	void sendCommandWithArgs(const char* cmd, const char* args, void* self){
		mock_c()->expectOneCall("cmdCallback")->withPointerParameters("self", self)->withStringParameters("args", args);
		string str = string(cmd) + string(args);
		for (size_t i=0; i<str.size(); i++){
			Terminal_putCh(str[i]);
		}
	}
};

extern "C"{
static void cmdCallback(void* self, const char* args){
	mock_c()->actualCall("cmdCallback")->withPointerParameters("self", self)->withStringParameters("args", args);
}
}

TEST(TerminalTestGroup, CanRecognizeCmdWithOutArg) {
	TerminalCmd cmd;
	cmd.cmdCallback = &cmdCallback;
	strncpy((char*)cmd.cmdStr, "cmd0 ", TERMINAL_MAX_CMD_LENGTH);
	cmd.numberOfArguments = 0;
	cmd.self = NULL;
	uint8_t id = Terminal_registerCmd(cmd);
	CHECK(id!=TERMINAL_INVALID_CMD_ID);

	sendCommandWithArgs("cmd0 ", "", NULL);
	Terminal_tick(0);

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

	sendCommandWithArgs("cmd1 ", "1 ", NULL);
	Terminal_tick(0);

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

	sendCommandWithArgs("cmd2 ", "1 2 ", NULL);
	Terminal_tick(0);

	mock_c()->checkExpectations();
}

