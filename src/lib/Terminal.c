/*
 * Terminal.c
 *
 *  Created on: Sep 28, 2014
 *      Author: jonasbachli
 */

#include "Terminal.h"
#include <string.h>
#include <stdio.h>
#include "FiFo.h"

#define FIFO_BUFFER_SIZE		(128)
#define MAX_LINE_LENGTH			(128)

#define MAX_STEPS_PER_TICK		(256)

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

typedef struct _TerminalCmdEntry {
	TerminalCmd cmd;
	bool isUsed;
} TerminalCmdEntry;

typedef bool (*stateFct)(void);

static bool checkCmdStructValid(TerminalCmd* cmd);
static void findCurrentCmd(void);
static void resetState(void);

static bool getLine(void);
static bool getCmd(void);
static bool getArguments(void);

static void installHelp(void);
static void handleHelp(void* self, const char* args, uint8_t argStrLen);
static void printLine(char* str);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static TerminalCmdEntry cmdRegistry[TERMINAL_MAX_NBR_OF_CMDS];
static TerminalSendCharCallback sendChar;

static uint8_t fifoBuffer[FIFO_BUFFER_SIZE];
static FiFo fifo;

static stateFct state;
static uint8_t currentLineStr[MAX_LINE_LENGTH];
static uint8_t currentLineStrIndex;
static uint8_t currentCmdIndex;
static uint8_t currentCmdStr[TERMINAL_MAX_CMD_LENGTH];
static uint8_t currentCmdStrIndex;
static uint8_t currentArgStr[TERMINAL_MAX_ARG_LENGTH];
static uint8_t currentArgStrIndex;
static uint8_t remainingNbrOfArgs;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void Terminal_init(void) {
	for (int i = 0; i < TERMINAL_MAX_NBR_OF_CMDS; i++) {
		cmdRegistry[i].isUsed = false;
	}
	sendChar = NULL;
	FiFo_init(&fifo, fifoBuffer, sizeof(uint8_t), FIFO_BUFFER_SIZE);
	resetState();

	installHelp();
}

uint8_t Terminal_registerCmd(TerminalCmd cmd) {
	if (checkCmdStructValid(&cmd)) {
		for (uint8_t i = 0; i < TERMINAL_MAX_NBR_OF_CMDS; i++) {
			if (!cmdRegistry[i].isUsed) {
				cmdRegistry[i].isUsed = true;
				cmdRegistry[i].cmd = cmd;
				return i;
			}
		}
	}
	return TERMINAL_INVALID_CMD_ID;
}

void Terminal_unregisterCmd(uint8_t id) {
	if (id < TERMINAL_MAX_NBR_OF_CMDS) {
		cmdRegistry[id].isUsed = false;
	}
}

void Terminal_initCmdStruct(TerminalCmd* cmd, uint8_t* cmdStr,
		TerminalCmdCallback cmdCallback, void* self, uint8_t numberOfArguments){
	strncpy((char*)cmd->cmdStr, (char*)cmdStr, TERMINAL_MAX_CMD_LENGTH);
	cmd->cmdCallback = cmdCallback;
	cmd->self = self;
	cmd->numberOfArguments = numberOfArguments;
}

void Terminal_tick(uint32_t timeInMs) {
	uint16_t ctr = MAX_STEPS_PER_TICK;
	while (state && state() && ctr--) {}
}

void Terminal_setSendCharCallback(TerminalSendCharCallback callback) {
	sendChar = callback;
}

void Terminal_putCh(uint8_t c) {
	FiFo_push(&fifo, &c);
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static bool checkCmdStructValid(TerminalCmd* cmd) {
	bool result;
	result = (strlen((char*) cmd->cmdStr) < TERMINAL_MAX_CMD_LENGTH);
	result = result && cmd->cmdCallback != NULL;
	return result;
}

static void findCurrentCmd(void) {
	for (uint8_t i = 0; i < TERMINAL_MAX_NBR_OF_CMDS; i++) {
		if (cmdRegistry[i].isUsed
				&& 0 == strncmp((char*) cmdRegistry[i].cmd.cmdStr,
								(char*) currentCmdStr,
								TERMINAL_MAX_CMD_LENGTH)) {
			currentCmdIndex = i;
			remainingNbrOfArgs = cmdRegistry[i].cmd.numberOfArguments;
			if (remainingNbrOfArgs)
				state = &getArguments;
			else {
				cmdRegistry[currentCmdIndex].cmd.cmdCallback(
						cmdRegistry[currentCmdIndex].cmd.self, NULL, 0);
				state = &getCmd;
			}
			return;
		}
	}
	resetState();
}

static void resetState(void) {
	state = &getLine;
	memset(currentLineStr, 0, MAX_LINE_LENGTH);
	memset(currentArgStr, 0, TERMINAL_MAX_ARG_LENGTH);
	memset(currentCmdStr, 0, TERMINAL_MAX_CMD_LENGTH);
	currentLineStrIndex = 0;
	currentArgStrIndex = 0;
	currentCmdStrIndex = 0;
	remainingNbrOfArgs = 0;
	currentCmdIndex = 0;
}

static bool getLine(void){
	uint8_t c;
	if (!FiFo_pop(&fifo, &c))
		return false;

	if (sendChar)
		sendChar(c);

	if (c=='\n' || c=='\r'){
		currentLineStr[currentLineStrIndex] = ' ';
		currentLineStrIndex = 0;

		if (sendChar && '\r'==c)
			sendChar('\n');
		state = &getCmd;
	}
	else if ('\b' == c){
		currentLineStr[currentLineStrIndex] = '\0';

		if (currentLineStrIndex>0)
			currentLineStrIndex--;
		if (sendChar){
			sendChar(' ');
			sendChar('\b');
		}
	}
	else {
		currentLineStr[currentLineStrIndex++] = c;

		if (MAX_LINE_LENGTH <= currentLineStrIndex){
			resetState();
			return false;
		}
	}
	return true;
}

static bool getCmd(void) {
	uint8_t c = currentLineStr[currentLineStrIndex++];
	currentCmdStr[currentCmdStrIndex++] = c;

	if (' ' == c)
		findCurrentCmd();
	else if (TERMINAL_MAX_CMD_LENGTH == currentCmdStrIndex){
		resetState();
		return false;
	}

	if (MAX_LINE_LENGTH <= currentLineStrIndex){
		resetState();
		return false;
	}

	return true;
}

static bool getArguments(void) {
	uint8_t c = currentLineStr[currentLineStrIndex++];
	currentArgStr[currentArgStrIndex++] = c;
	if (' ' == c) {
		remainingNbrOfArgs--;
		if (0 == remainingNbrOfArgs) {
			TerminalCmd* cmd = &cmdRegistry[currentCmdIndex].cmd;
			cmd->cmdCallback(cmd->self, (const char*) currentArgStr, currentArgStrIndex-1);
			resetState();
			return false;
		}
	} else if (TERMINAL_MAX_ARG_LENGTH == currentArgStrIndex){
		resetState();
		return false;
	}

	if (MAX_LINE_LENGTH <= currentLineStrIndex) {
		resetState();
		return false ;
	}
	return true;
}

static void installHelp(void){
	TerminalCmd cmd;
	Terminal_initCmdStruct(&cmd, "? ", &handleHelp, 0, 0);
	Terminal_registerCmd(cmd);
}

static void handleHelp(void* self, const char* args, uint8_t argStrLen){
	(void) self;
	(void) args;
	(void) argStrLen;
	char buffer[100];
	snprintf(buffer, 100,
			"Registered commands: <cmd> <nbrOfArgs>\r\n---------------------------------------");
	printLine(buffer);
	for (uint8_t i = 0; i < TERMINAL_MAX_NBR_OF_CMDS; i++) {
		if (cmdRegistry[i].isUsed && 0 != strncmp("? ", cmdRegistry[i].cmd.cmdStr, 2)) {
			snprintf(buffer, 100, "%s %d", (char*) cmdRegistry[i].cmd.cmdStr,
					cmdRegistry[i].cmd.numberOfArguments);
			printLine(buffer);
		}
	}
}

static void printLine(char* str){
	uint8_t len = strlen(str);
	if (sendChar){
		for (uint8_t i = 0; i < len; i++)
			sendChar(str[i]);
		sendChar('\r');
		sendChar('\n');
	}
}
