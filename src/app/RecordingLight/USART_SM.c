/*
 * USART_SM.c
 *
 *  Created on: Jul 15, 2014
 *      Author: jonasbachli
 */

#include "USART_SM.h"

#include "USART1.h"
#include "FiFo.h"

#include <string.h>

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

#define RECEIVE_BUFFER_SIZE		(128)

typedef void (*stateFct)(uint8_t byte);

static void receiveByteCallback(uint8_t byte);

static void readHeader(uint8_t byte);
static void readCommand(uint8_t byte);
static void readArg(uint8_t byte);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static RLMsg decodedMsg;
static bool gotNewDecodedMessage;
volatile static uint8_t receiveBuffer[RECEIVE_BUFFER_SIZE];
volatile static FiFo receiveFiFo;
static uint8_t argReadCtr;
static stateFct currentState;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void USART_SM_init(void){
	FiFo_init(&receiveFiFo, (void*) receiveBuffer, 1, RECEIVE_BUFFER_SIZE);

	gotNewDecodedMessage = false;
	currentState = &readHeader;

	USART1_setByteReceivedCallback(&receiveByteCallback);
	USART1_init();
}

void USART_SM_tick(void){
	uint8_t dataByte;
	while (FiFo_pop(&receiveFiFo, &dataByte) && !gotNewDecodedMessage){
		currentState(dataByte);
	}
}

bool USART_SM_getMessage(RLMsg* msg){
	if (gotNewDecodedMessage){
		gotNewDecodedMessage = false;
		memcpy(msg, &decodedMsg, sizeof(RLMsg));
		return true;
	}
	return false;
}

void USART_SM_sendMessage(RLMsg* msg){
	USART1_sendByte(RL_RESPONSE_HEADER);
	USART1_sendByte(msg->cmd);
	USART1_sendByte((msg->arg >> 24) & 0x000000ff);
	USART1_sendByte((msg->arg >> 16) & 0x000000ff);
	USART1_sendByte((msg->arg >> 8) & 0x000000ff);
	USART1_sendByte((msg->arg) & 0x000000ff);
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static void receiveByteCallback(uint8_t byte){
	FiFo_push(&receiveFiFo, &byte);
}

static void readHeader(uint8_t byte){
	if (byte == RL_CMD_HEADER){
		currentState = &readCommand;
	}
}

static void readCommand(uint8_t byte){
	decodedMsg.cmd = byte;
	currentState = &readArg;
	argReadCtr = 0;
	decodedMsg.arg = 0;
}

static void readArg(uint8_t byte){
	uint8_t shiftCount = 4-argReadCtr;
	decodedMsg.arg |= (0x000000ff<<shiftCount) & ( (uint32_t)byte << shiftCount);
	argReadCtr++;
	if (argReadCtr >= 4){
		gotNewDecodedMessage = true;
		currentState = &readHeader;
	}
}
