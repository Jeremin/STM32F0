/*
 * USART1.h
 *
 *  Created on: Jul 15, 2014
 *      Author: jonasbachli
 */

#ifndef USART1_H_
#define USART1_H_


#include <stdint.h>
#include <stdbool.h>

typedef void (*USART_receivedByteCbFct)(uint8_t byte);

void USART1_init(void);
void USART1_setByteReceivedCallback(USART_receivedByteCbFct receivedByteCB);
void USART1_sendByte(uint8_t data);

#endif /* USART1_H_ */
