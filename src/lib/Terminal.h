/*
 * Terminal.h
 *
 *  Created on: Sep 28, 2014
 *      Author: jonasbachli
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <stdint.h>
#include <stdbool.h>

#define TERMINAL_MAX_CMD_LENGTH		(32)
#define TERMINAL_MAX_ARG_LENGTH		(64)
#define TERMINAL_MAX_NBR_OF_CMDS	(16)
#define TERMINAL_INVALID_CMD_ID		(0xff)

typedef void (*TerminalCmdCallback)(void* self, const char* args, uint8_t argStrLen);
typedef void (*TerminalSendCharCallback)(uint8_t c);

typedef struct _TerminalCmd{
	uint8_t cmdStr[TERMINAL_MAX_CMD_LENGTH];
	TerminalCmdCallback cmdCallback;
	void* self;
	uint8_t numberOfArguments;
} TerminalCmd;

void Terminal_init(void);
uint8_t Terminal_registerCmd(TerminalCmd cmd);
void Terminal_unregisterCmd(uint8_t id);

void Terminal_initCmdStruct(TerminalCmd* cmd, uint8_t* cmdStr,
		TerminalCmdCallback cmdCallback, void* self, uint8_t numberOfArguments);

void Terminal_tick(uint32_t timeInMs);

void Terminal_setSendCharCallback(TerminalSendCharCallback callback);
void Terminal_putCh(uint8_t c);


#endif /* TERMINAL_H_ */
