/*
 * FiFo.h
 *
 *  Created on: Jul 15, 2014
 *      Author: jonasbachli
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct FiFo_t {
	void* dataPtr;
	uint16_t elemSize;
	uint16_t memSize;
	void* startPtr;
	void* endPtr;
	uint16_t size;
	uint16_t maxSize;
} FiFo;

void FiFo_init(volatile FiFo* fifo, void* dataPtr, uint16_t elemSize, uint16_t memSize);

bool FiFo_push(volatile FiFo* fifo, void* data);
bool FiFo_pop(volatile FiFo* fifo, void* data);

uint16_t FiFo_size(volatile FiFo* fifo);

#endif /* FIFO_H_ */
