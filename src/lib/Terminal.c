/*
 * Terminal.c
 *
 *  Created on: Sep 28, 2014
 *      Author: jonasbachli
 */

#include "Terminal.h"
#include <string.h>
#include "FiFo.h"

#define FIFO_BUFFER_SIZE		(128)

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

typedef struct _TerminalCmdEntry{
	TerminalCmd cmd;
	bool isUsed;
} TerminalCmdEntry;

typedef void (*stateFct)(uint8_t c);

static bool checkCmdStructValid(TerminalCmd* cmd);
static void findCurrentCmd(void);
static void resetState(void);

static void getCmd(uint8_t c);
static void getArguments(uint8_t c);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static TerminalCmdEntry cmdRegistry[TERMINAL_MAX_NBR_OF_CMDS];
static TerminalSendCharCallback sendChar;

static uint8_t fifoBuffer[FIFO_BUFFER_SIZE];
static FiFo fifo;

static stateFct state;
static uint8_t currentCmdIndex;
static uint8_t currentCmdStr[TERMINAL_MAX_CMD_LENGTH];
static uint8_t currentCmdStrIndex;
static uint8_t currentArgStr[TERMINAL_MAX_ARG_LENGTH];
static uint8_t currentArgStrIndex;
static uint8_t remainingNbrOfArgs;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void Terminal_init(void){
	for (int i=0; i<TERMINAL_MAX_NBR_OF_CMDS; i++){
		cmdRegistry[i].isUsed = false;
	}
	sendChar = NULL;
	FiFo_init(&fifo, fifoBuffer, sizeof(uint8_t), FIFO_BUFFER_SIZE);
	resetState();
}

uint8_t Terminal_registerCmd(TerminalCmd cmd){
	if (checkCmdStructValid(&cmd)){
		for (uint8_t i=0; i<TERMINAL_MAX_NBR_OF_CMDS; i++){
			if (!cmdRegistry[i].isUsed){
				cmdRegistry[i].isUsed = true;
				cmdRegistry[i].cmd = cmd;
				return i;
			}
		}
	}
	return TERMINAL_INVALID_CMD_ID;
}

void Terminal_unregisterCmd(uint8_t id){
	if (id < TERMINAL_MAX_NBR_OF_CMDS){
		cmdRegistry[id].isUsed = false;
	}
}

void Terminal_tick(uint32_t timeInMs) {
	while (state && FiFo_size(&fifo)) {
		uint8_t c;
		FiFo_pop(&fifo, &c);
		state(c);
		if (sendChar)
			sendChar(c);
	}
}

void Terminal_setSendCharCallback(TerminalSendCharCallback callback){

}

void Terminal_putCh(uint8_t c){
	FiFo_push(&fifo, &c);
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static bool checkCmdStructValid(TerminalCmd* cmd){
	bool result;
	result = (strlen((char*)cmd->cmdStr) < TERMINAL_MAX_CMD_LENGTH);
	result = result && cmd->cmdCallback!=NULL;
	return result;
}

static void findCurrentCmd(void){
	for (uint8_t i=0; i<TERMINAL_MAX_NBR_OF_CMDS; i++){
		if (cmdRegistry[i].isUsed && 0==strncmp((char*)cmdRegistry[i].cmd.cmdStr, (char*)currentCmdStr, TERMINAL_MAX_CMD_LENGTH)){
			currentCmdIndex = i;
			remainingNbrOfArgs = cmdRegistry[i].cmd.numberOfArguments;
			if (remainingNbrOfArgs)
				state = &getArguments;
			else {
				cmdRegistry[currentCmdIndex].cmd.cmdCallback(cmdRegistry[currentCmdIndex].cmd.self, NULL);
				state = &getCmd;
			}
			return;
		}
	}
	resetState();
}

static void resetState(void){
	state = &getCmd;
	memset(currentArgStr, 0, TERMINAL_MAX_ARG_LENGTH);
	memset(currentCmdStr, 0, TERMINAL_MAX_CMD_LENGTH);
	currentArgStrIndex = 0;
	currentCmdStrIndex = 0;
	remainingNbrOfArgs = 0;
	currentCmdIndex = 0;
}

static void getCmd(uint8_t c){
	currentCmdStr[currentCmdStrIndex++] = c;
	if (' ' == c)
		findCurrentCmd();
	else if (TERMINAL_MAX_CMD_LENGTH==currentCmdStrIndex)
		resetState();
}

static void getArguments(uint8_t c){
	currentArgStr[currentArgStrIndex++] = c;
	if (' ' == c){
		remainingNbrOfArgs--;
		if (0 == remainingNbrOfArgs){
			TerminalCmd* cmd = &cmdRegistry[currentCmdIndex].cmd;
			cmd->cmdCallback(cmd->self, (const char*)currentArgStr);
			resetState();
		}
	} else if (TERMINAL_MAX_ARG_LENGTH==currentArgStrIndex)
		resetState();
}
