/*
 * System.c
 *
 *  Created on: Jul 11, 2014
 *      Author: jonasbachli
 */

#include "System.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

void idle(void);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static uint32_t localTime = 0;
static taskCallbackFct taskCallback;
static taskCallbackFct idleCallback;
static volatile bool handleSystickInterrupt = false;
static uint32_t systickIncrementInMs;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void System_init(void) {

}

void System_PITSetup(uint32_t systickInMs) {
	RCC_ClocksTypeDef rccClocks;

	systickIncrementInMs = systickInMs;
	RCC_GetClocksFreq(&rccClocks);
	SysTick_Config((rccClocks.HCLK_Frequency / 1000) * systickInMs);
}

void System_PITSetTaskCallback(taskCallbackFct taskCB) {
	taskCallback = taskCB;
}

void System_PITSetIdleCallback(taskCallbackFct idleCB) {
	idleCallback = idleCB;
}

void System_PITStart(void) {
	while(1){
		handleSystickInterrupt = false;
		if (taskCallback){
			taskCallback(localTime);
		}

		localTime += systickIncrementInMs;

		idle();
	}
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

void idle(void){
	while (!handleSystickInterrupt){
		if (idleCallback){
			idleCallback(localTime);
		}
	}
}

void SysTick_Handler(void)
{
	handleSystickInterrupt = true;
}
