/*
 * DMA1.h
 *
 *  Created on: Jul 23, 2014
 *      Author: jonasbachli
 */

#ifndef DMA1_H_
#define DMA1_H_

#include <stdint.h>
#include <stdbool.h>

typedef void (*DMA_Callback)(void* arg);

void DMA1_init(void);

void DMA1_setCallbacks(DMA_Callback transferComplete,
		DMA_Callback halfTransferComplete, DMA_Callback error, void* arg,
		uint8_t channel);

#endif /* DMA1_H_ */
