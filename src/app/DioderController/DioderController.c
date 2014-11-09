/*
 * DioderController.c
 *
 *  Created on: Sep 28, 2014
 *      Author: jonasbachli
 */

#include "DioderController.h"

#include "Terminal.h"
#include "USART1.h"
#include "Tim1_Pwm.h"
#include "TypesAndMacros.h"
#include "ArgParser.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

static void registerCommands(void);
static void redCallback(void* self, const char* args, uint8_t argStrLen);
static void greenCallback(void* self, const char* args, uint8_t argStrLen);
static void blueCallback(void* self, const char* args, uint8_t argStrLen);

static void onRedParsed(char* arg, uint8_t argLength, uint8_t argIndex);
static void onGreenParsed(char* arg, uint8_t argLength, uint8_t argIndex);
static void onBlueParsed(char* arg, uint8_t argLength, uint8_t argIndex);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static uint8_t red;
static uint8_t green;
static uint8_t blue;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void DioderController_init(void){
	red = 0;
	green = 0;
	blue = 0;

	USART1_init();
	Tim1_Pwm_init();
	Terminal_init();
	registerCommands();
	Terminal_setSendCharCallback(&USART1_sendByte);
	USART1_setByteReceivedCallback(&Terminal_putCh);
}

void DioderController_tick(uint32_t timeInMs){
	Terminal_tick(timeInMs);
	Tim1_Pwm_setValueCh1(red);
	Tim1_Pwm_setValueCh2(green);
	Tim1_Pwm_setValueCh3(blue);
}

/////////////////////////////////////
// private functions
/////////////////////////////////////


static void registerCommands(void){
	TerminalCmd cmd;

	Terminal_initCmdStruct(&cmd, "red ", &redCallback, NULL, 1);
	Terminal_registerCmd(cmd);
	Terminal_initCmdStruct(&cmd, "green ", &greenCallback, NULL, 1);
	Terminal_registerCmd(cmd);
	Terminal_initCmdStruct(&cmd, "blue ", &blueCallback, NULL, 1);
	Terminal_registerCmd(cmd);
}

static void redCallback(void* self, const char* args, uint8_t argStrLen){
	(void) self;

	ArgParser argParser;
	ArgParser_init(&argParser, (char*)args, argStrLen, &onRedParsed);
	ArgParser_parse(&argParser);
}

static void greenCallback(void* self, const char* args, uint8_t argStrLen){
	(void) self;

		ArgParser argParser;
		ArgParser_init(&argParser, (char*)args, argStrLen, &onGreenParsed);
		ArgParser_parse(&argParser);
}

static void blueCallback(void* self, const char* args, uint8_t argStrLen){
	(void) self;

		ArgParser argParser;
		ArgParser_init(&argParser, (char*)args, argStrLen, &onBlueParsed);
		ArgParser_parse(&argParser);
}

static void onRedParsed(char* arg, uint8_t argLength, uint8_t argIndex){
	red = ArgParser_getUint(arg, argLength);
}

static void onGreenParsed(char* arg, uint8_t argLength, uint8_t argIndex){
	green = ArgParser_getUint(arg, argLength);
}

static void onBlueParsed(char* arg, uint8_t argLength, uint8_t argIndex){
	blue = ArgParser_getUint(arg, argLength);
}
