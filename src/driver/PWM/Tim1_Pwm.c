/*
 * Tim1_Pwm.c
 *
 *  Created on: Aug 31, 2014
 *      Author: jonasbachli
 */

#include "Tim1_Pwm.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////

void Tim1_Pwm_init(void) {
	GPIO_InitTypeDef gpioInit;
	TIM_TimeBaseInitTypeDef timerTimeBaseInit;
	TIM_OCInitTypeDef timerOCInit;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInit);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_2);

	timerTimeBaseInit.TIM_Prescaler = 0;
	timerTimeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
	timerTimeBaseInit.TIM_Period = 256;
	timerTimeBaseInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timerTimeBaseInit.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerTimeBaseInit);

	timerOCInit.TIM_OCMode = TIM_OCMode_PWM1;
	timerOCInit.TIM_OutputState = TIM_OutputState_Enable;
	timerOCInit.TIM_OutputNState = TIM_OutputNState_Enable;
	timerOCInit.TIM_OCPolarity = TIM_OCPolarity_High;
	timerOCInit.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	timerOCInit.TIM_OCIdleState = TIM_OCIdleState_Set;
	timerOCInit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	timerOCInit.TIM_Pulse = 0;

	TIM_OC1Init(TIM1, &timerOCInit);
	TIM_OC2Init(TIM1, &timerOCInit);
	TIM_OC3Init(TIM1, &timerOCInit);
	TIM_OC4Init(TIM1, &timerOCInit);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	TIM_Cmd(TIM1, ENABLE);
}

void Tim1_Pwm_setValueCh1(uint8_t value) {
	TIM_SetCompare1(TIM1, value);
}

void Tim1_Pwm_setValueCh2(uint8_t value) {
	TIM_SetCompare2(TIM1, value);
}

void Tim1_Pwm_setValueCh3(uint8_t value) {
	TIM_SetCompare3(TIM1, value);
}

void Tim1_Pwm_setValueCh4(uint8_t value) {
	TIM_SetCompare4(TIM1, value);
}
/////////////////////////////////////
// private functions
/////////////////////////////////////

