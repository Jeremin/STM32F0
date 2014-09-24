/*
 * Flash.h
 *
 *  Created on: Jul 20, 2014
 *      Author: jonasbachli
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>
#include <stdbool.h>

void Flash_init(void);

void Flash_writeToDataPage(void* data, uint32_t size);
void Flash_readFromDataPage(void* data, uint32_t size);

#endif /* FLASH_H_ */
