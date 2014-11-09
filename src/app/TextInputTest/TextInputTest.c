/*
 * TextInputTest.c
 *
 *  Created on: Nov 9, 2014
 *      Author: jonasbachli
 */

#include "TextInputTest.h"
#include "USART1.h"

#define MAX_STR_SIZE	100

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

static void receivedByte(uint8_t byte);

/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////

void TextInputTest_init(void){
	USART1_setByteReceivedCallback(&receivedByte);
	USART1_init();
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static void receivedByte(uint8_t byte){
	if ('\r' == byte)
		USART1_sendByte('\n');
	if ('\b' == byte){
		USART1_sendByte(byte);
		USART1_sendByte(' ');
		USART1_sendByte(byte);
		return;
	}
	USART1_sendByte(byte);
}
