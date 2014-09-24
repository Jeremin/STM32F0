/*
 * DMA1.c
 *
 *  Created on: Jul 23, 2014
 *      Author: jonasbachli
 */

#include "DMA1.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_dma.h"

#define DMA1_NBR_OF_CHANNELS		5

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

/////////////////////////////////////
// private variables
/////////////////////////////////////
static DMA_Callback errorCallback[DMA1_NBR_OF_CHANNELS];
static DMA_Callback transferCompleteCallback[DMA1_NBR_OF_CHANNELS];
static DMA_Callback halfTransferCompleteCallback[DMA1_NBR_OF_CHANNELS];
static void* callbackArg[DMA1_NBR_OF_CHANNELS];
/////////////////////////////////////
// public functions
/////////////////////////////////////
void DMA1_init(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void DMA1_setCallbacks(DMA_Callback transferComplete,
		DMA_Callback halfTransferComplete, DMA_Callback error, void* arg,
		uint8_t channel) {
	if (channel < DMA1_NBR_OF_CHANNELS) {
		transferCompleteCallback[channel] = transferComplete;
		halfTransferCompleteCallback[channel] = halfTransferComplete;
		errorCallback[channel] = error;
		callbackArg[channel] = arg;
	}
}
/////////////////////////////////////
// private functions
/////////////////////////////////////

void DMA1_Channel1_IRQHandler() {
	if (DMA_GetITStatus(DMA1_IT_TC1) && transferCompleteCallback[0]) {
		transferCompleteCallback[0](callbackArg[0]);
	}
	if (DMA_GetITStatus(DMA1_IT_HT1) && halfTransferCompleteCallback[0]) {
		halfTransferCompleteCallback[0](callbackArg[0]);
	}
	if (DMA_GetITStatus(DMA1_IT_TE1) && errorCallback[0]) {
		errorCallback[0](callbackArg[0]);
	}

	DMA_ClearITPendingBit(DMA1_IT_GL1);
}

void DMA1_Channel2_3_IRQHandler() {
	//CH 2
	if (DMA_GetITStatus(DMA1_IT_TC2) && transferCompleteCallback[1]) {
		transferCompleteCallback[1](callbackArg[1]);
	}
	if (DMA_GetITStatus(DMA1_IT_HT2) && halfTransferCompleteCallback[1]) {
		halfTransferCompleteCallback[1](callbackArg[1]);
	}
	if (DMA_GetITStatus(DMA1_IT_TE2) && errorCallback[1]) {
		errorCallback[1](callbackArg[1]);
	}

	DMA_ClearITPendingBit(DMA1_IT_GL2);

	//CH 3
	if (DMA_GetITStatus(DMA1_IT_TC3) && transferCompleteCallback[2]) {
		transferCompleteCallback[2](callbackArg[2]);
	}
	if (DMA_GetITStatus(DMA1_IT_HT3) && halfTransferCompleteCallback[2]) {
		halfTransferCompleteCallback[2](callbackArg[2]);
	}
	if (DMA_GetITStatus(DMA1_IT_TE3) && errorCallback[2]) {
		errorCallback[2](callbackArg[2]);
	}

	DMA_ClearITPendingBit(DMA1_IT_GL3);
}

void DMA1_Channel4_5_IRQHandler() {
	//CH 4
	if (DMA_GetITStatus(DMA1_IT_TC4) && transferCompleteCallback[3]) {
		transferCompleteCallback[3](callbackArg[3]);
	}
	if (DMA_GetITStatus(DMA1_IT_HT4) && halfTransferCompleteCallback[3]) {
		halfTransferCompleteCallback[3](callbackArg[3]);
	}
	if (DMA_GetITStatus(DMA1_IT_TE4) && errorCallback[3]) {
		errorCallback[3](callbackArg[3]);
	}

	DMA_ClearITPendingBit(DMA1_IT_GL4);

	//CH 5
	if (DMA_GetITStatus(DMA1_IT_TC5) && transferCompleteCallback[4]) {
		transferCompleteCallback[4](callbackArg[4]);
	}
	if (DMA_GetITStatus(DMA1_IT_HT5) && halfTransferCompleteCallback[4]) {
		halfTransferCompleteCallback[4](callbackArg[4]);
	}
	if (DMA_GetITStatus(DMA1_IT_TE5) && errorCallback[4]) {
		errorCallback[4](callbackArg[4]);
	}

	DMA_ClearITPendingBit(DMA1_IT_GL5);
}
