/*
 * Flash.c
 *
 *  Created on: Jul 20, 2014
 *      Author: jonasbachli
 */


#include "Flash.h"

#include "stm32f0xx_flash.h"
#include <string.h>

/////////////////////////////////////
// forward declarations
/////////////////////////////////////
#define DATA_PAGE_START_ADDR	((uint32_t) 0x0800FC00)
/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////
void Flash_init(void){
	FLASH_Unlock();
}

void Flash_writeToDataPage(void* data, uint32_t size){
	uint8_t remainder;
	uint16_t i;
	uint32_t remainderData = 0;

	remainder = size % 4;
	size /= 4;

	FLASH_ErasePage(DATA_PAGE_START_ADDR);

	for (i=0; i<size; i++){
		FLASH_ProgramWord(DATA_PAGE_START_ADDR + i*4, (uint32_t) *((uint32_t*)data+i*4));
	}

	if (remainder){
		for (i=0; i<remainder; i++){
			((uint8_t*)&remainderData)[i] = (uint8_t) *((uint8_t*)data+size*4+i);
		}
		FLASH_ProgramWord(DATA_PAGE_START_ADDR + size*4, remainderData);
	}
}

void Flash_readFromDataPage(void* data, uint32_t size){
	memcpy(data, (void*)DATA_PAGE_START_ADDR, size);
}
/////////////////////////////////////
// private functions
/////////////////////////////////////

