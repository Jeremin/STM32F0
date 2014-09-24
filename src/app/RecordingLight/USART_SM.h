/*
 * USART_SM.h
 *
 *  Created on: Jul 15, 2014
 *      Author: jonasbachli
 */

#ifndef USART_SM_H_
#define USART_SM_H_

#include "SerialProtocol.h"

#include <stdint.h>
#include <stdbool.h>

void USART_SM_init(void);
void USART_SM_tick(void);

bool USART_SM_getMessage(RLMsg* msg);
void USART_SM_sendMessage(RLMsg* msg);

#endif /* USART_SM_H_ */
