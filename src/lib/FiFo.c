/*
 * FiFo.c
 *
 *  Created on: Jul 16, 2014
 *      Author: jonasbachli
 */

#include "FiFo.h"
#include <string.h>

/////////////////////////////////////
// forward declarations
/////////////////////////////////////
static void* getNextPointerLocation(volatile FiFo* fifo, void* currentPtr);
/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////
void FiFo_init(volatile FiFo* fifo, void* dataPtr, uint16_t elemSize, uint16_t memSize) {
	fifo->dataPtr = dataPtr;
	fifo->elemSize = elemSize;
	fifo->startPtr = dataPtr;
	fifo->endPtr = dataPtr;
	fifo->memSize = memSize;
	fifo->size = 0;
	fifo->maxSize = memSize/elemSize;
}

bool FiFo_push(volatile FiFo* fifo, void* data) {
	if (fifo->size < fifo->maxSize){
		memcpy(fifo->endPtr, data, fifo->elemSize);
		fifo->endPtr = getNextPointerLocation(fifo, fifo->endPtr);
		fifo->size++;
		return true;
	}
	return false;
}

bool FiFo_pop(volatile FiFo* fifo, void* data) {
	if (fifo->size){
		memcpy(data, fifo->startPtr, fifo->elemSize);
		fifo->startPtr = getNextPointerLocation(fifo, fifo->startPtr);
		fifo->size--;
		return true;
	}
	return false;
}

uint16_t FiFo_size(volatile FiFo* fifo) {
	return fifo->size;
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

static void* getNextPointerLocation(volatile FiFo* fifo, void* currentPtr){
	if (currentPtr <= (fifo->dataPtr + (fifo->maxSize-1)*fifo->elemSize)){
		return currentPtr + fifo->elemSize;
	}
	else {
		return fifo->dataPtr;
	}
}
