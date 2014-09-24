/*
 * USART1.c
 *
 *  Created on: Jul 15, 2014
 *      Author: jonasbachli
 */

#include "USART1.h"

#include <stm32f0xx_usart.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_gpio.h>

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

/////////////////////////////////////
// private variables
/////////////////////////////////////

static USART_receivedByteCbFct receivedByteCallback = 0;

/////////////////////////////////////
// public functions
/////////////////////////////////////

void USART1_init(void) {
	GPIO_InitTypeDef gpioInit;
	USART_InitTypeDef usartInit;
	NVIC_InitTypeDef nvicInit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	gpioInit.GPIO_Mode = GPIO_Mode_AF;
	gpioInit.GPIO_OType = GPIO_OType_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpioInit);

	usartInit.USART_BaudRate = 38400;
	usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usartInit.USART_Parity = USART_Parity_No;
	usartInit.USART_StopBits = USART_StopBits_1;
	usartInit.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usartInit);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	nvicInit.NVIC_IRQChannel = USART1_IRQn;
	nvicInit.NVIC_IRQChannelPriority = 1;
	nvicInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInit);

	USART_Cmd(USART1, ENABLE);
}

void USART1_setByteReceivedCallback(USART_receivedByteCbFct receivedByteCB) {
	receivedByteCallback = receivedByteCB;
}

void USART1_sendByte(uint8_t data) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USART1, data);
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		uint8_t receivedValue = (uint8_t) USART1->RDR & 0x00ff;

		if (receivedByteCallback) {
			receivedByteCallback(receivedValue);
		}
	}

	USART_ClearFlag(USART1, USART_FLAG_WU | USART_FLAG_CM | USART_FLAG_EOB | USART_FLAG_RTO | USART_FLAG_CTS | USART_FLAG_LBD | USART_FLAG_TC | USART_FLAG_IDLE | USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
}
