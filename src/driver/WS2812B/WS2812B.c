/*
 * WS2812B.c
 *
 *  Created on: Jul 23, 2014
 *      Author: jonasbachli
 */

#include "WS2812B.h"

#include "DMA1.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_dma.h"

#include "TypesAndMacros.h"

#define TIMER_DIV					(1)
#define TIMER_PERIOD				(63)
#define TIMER_0_DUTY_CYCLE			(20)
#define TIMER_1_DUTY_CYCLE			(40)
#define TIMER_RESET_DUTY_CYLCE 		(0)
#define TIMER_RESET_REPEAT_COUNT	(39)

#define DUTY_CYCLE_BUFFER_SIZE		WS2812B_NBR_OF_LEDS*3*8 + TIMER_RESET_REPEAT_COUNT

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

static void updateDutyCycleBuffer(void);

static void initGpio(void);
static void initPwmTimer(void);
static void initDma(void);

static void dmaTransferCompleteHandler(void* arg);
static void dmaErrorHandler(void* arg);

/////////////////////////////////////
// private variables
/////////////////////////////////////

static RGB ledBuffer[WS2812B_NBR_OF_LEDS];
static volatile uint8_t dutyCycleBuffer[DUTY_CYCLE_BUFFER_SIZE];
static uint32_t numberOfActiveLeds;
static volatile uint16_t currentDutyCycleBufferSize;
static volatile bool isRunning;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void WS2812B_init() {
	numberOfActiveLeds = WS2812B_NBR_OF_LEDS;
	currentDutyCycleBufferSize = 0;
	isRunning = false;

	initGpio();
	initPwmTimer();
	initDma();
}

WS2812BLedDescription WS2812B_getLedDescription(void) {
	WS2812BLedDescription desc;
	desc.leds = ledBuffer;
	desc.nbrOfLeds = WS2812B_NBR_OF_LEDS;
	return desc;
}

void WS2812B_setLedColor(RGB* color, uint32_t ledIndex) {
	if (ledIndex < WS2812B_NBR_OF_LEDS) {
		ledBuffer[ledIndex].red = color->red;
		ledBuffer[ledIndex].green = color->green;
		ledBuffer[ledIndex].blue = color->blue;
	}
}

RGB* WS2812B_getLedColor(uint32_t ledIndex) {
	if (ledIndex < WS2812B_NBR_OF_LEDS) {
		return ledBuffer + ledIndex;
	} else {
		return 0;
	}
}

void WS2812B_fillColor(RGB* color) {
	uint32_t i;
	for (i = 0; i < WS2812B_NBR_OF_LEDS; i++) {
		ledBuffer[i].red = color->red;
		ledBuffer[i].green = color->green;
		ledBuffer[i].blue = color->blue;
	}
}

void WS2812B_fillSectionColor(RGB* color, uint32_t firstLedIndex,
		uint32_t lastLedIndex) {
	uint32_t i;
	for (i = firstLedIndex; i <= lastLedIndex; i++) {
		ledBuffer[i].red = color->red;
		ledBuffer[i].green = color->green;
		ledBuffer[i].blue = color->blue;
	}
}

void WS2812B_setNumberOfActiveLeds(uint32_t nbrOfActiveLeds) {
	numberOfActiveLeds = nbrOfActiveLeds;
}

uint32_t WS2812B_getNumberOfActiveLeds(void) {
	return numberOfActiveLeds;
}

void WS2812B_tick(uint32_t localtime) {
	if (localtime % 50 == 0) {
		updateDutyCycleBuffer();
	}
}

void WS2812B_start(void) {
	updateDutyCycleBuffer();

	isRunning = true;

	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel2, currentDutyCycleBufferSize);
	DMA_Cmd(DMA1_Channel2, ENABLE);
}

void WS2812B_stop(void) {
	isRunning = false;
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static void updateDutyCycleBuffer(void) {
	uint32_t i;
	uint8_t j;

	uint32_t dcIndex = 0;

	for (i = 0; i < numberOfActiveLeds; i++) {
		for (j = 0; j < 8; j++) {
			if ((ledBuffer[i].green << j) & 0x80) {
				dutyCycleBuffer[dcIndex++] = TIMER_1_DUTY_CYCLE;
			} else {
				dutyCycleBuffer[dcIndex++] = TIMER_0_DUTY_CYCLE;
			}
		}
		for (j = 0; j < 8; j++) {
			if ((ledBuffer[i].red << j) & 0x80) {
				dutyCycleBuffer[dcIndex++] = TIMER_1_DUTY_CYCLE;
			} else {
				dutyCycleBuffer[dcIndex++] = TIMER_0_DUTY_CYCLE;
			}
		}
		for (j = 0; j < 8; j++) {
			if ((ledBuffer[i].blue << j) & 0x80) {
				dutyCycleBuffer[dcIndex++] = TIMER_1_DUTY_CYCLE;
			} else {
				dutyCycleBuffer[dcIndex++] = TIMER_0_DUTY_CYCLE;
			}
		}
	}

	for (i = 0; i < TIMER_RESET_REPEAT_COUNT; i++) {
		dutyCycleBuffer[dcIndex++] = TIMER_RESET_DUTY_CYLCE;
	}

	currentDutyCycleBufferSize = dcIndex;
}

static void initGpio(void) {
	GPIO_InitTypeDef gpioInit;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_0;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInit);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
}

static void initPwmTimer(void) {
	TIM_TimeBaseInitTypeDef timerTimeBaseInit;
	TIM_OCInitTypeDef timerOCInit;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	timerTimeBaseInit.TIM_Prescaler = 0;
	timerTimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerTimeBaseInit.TIM_Period = TIMER_PERIOD;
	timerTimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timerTimeBaseInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &timerTimeBaseInit);

	timerOCInit.TIM_OCMode = TIM_OCMode_PWM1;
	timerOCInit.TIM_OutputState = TIM_OutputState_Enable;
	timerOCInit.TIM_OutputNState = TIM_OutputNState_Enable;
	timerOCInit.TIM_OCPolarity = TIM_OCPolarity_Low;
	timerOCInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
	timerOCInit.TIM_OCIdleState = TIM_OCIdleState_Set;
	timerOCInit.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	timerOCInit.TIM_Pulse = 0;

	TIM_DMACmd(TIM3, TIM_DMA_CC3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

static void initDma(void) {
	DMA_InitTypeDef dmaInit;
	NVIC_InitTypeDef nvicInit;

	DMA1_init();

	DMA_DeInit(DMA1_Channel2);
	dmaInit.DMA_BufferSize = 1;
	dmaInit.DMA_DIR = DMA_DIR_PeripheralDST;
	dmaInit.DMA_M2M = DMA_M2M_Disable;
	dmaInit.DMA_MemoryBaseAddr = (uint32_t) dutyCycleBuffer;
	dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dmaInit.DMA_Mode = DMA_Mode_Normal;
	dmaInit.DMA_PeripheralBaseAddr = (uint32_t) &TIM3->CCR3;
	dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dmaInit.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel2, &dmaInit);

	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC | DMA_IT_TE, ENABLE);
	DMA1_setCallbacks(&dmaTransferCompleteHandler, NULL, &dmaErrorHandler, NULL, 1);

	nvicInit.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	nvicInit.NVIC_IRQChannelPriority = 1;
	NVIC_Init(&nvicInit);
}

static void dmaTransferCompleteHandler(void* arg) {
	DMA_Cmd(DMA1_Channel2, DISABLE);

	if (isRunning) {
		DMA_SetCurrDataCounter(DMA1_Channel2, currentDutyCycleBufferSize);
		DMA_Cmd(DMA1_Channel2, ENABLE);
	}
}

static void dmaErrorHandler(void* arg) {
	while (1)
		;
}
