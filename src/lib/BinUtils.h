/*
 * BinUtils.h
 *
 *  Created on: Jul 19, 2014
 *      Author: jonasbachli
 */

#ifndef BINUTILS_H_
#define BINUTILS_H_


#include <stdint.h>
#include <stdbool.h>

uint32_t BinUtils_create32BitFrom4Bytes(uint8_t firstByte, uint8_t secondByte, uint8_t thirdByte, uint8_t fourthByte);
void BinUtils_get4BytesFrom32Bit(uint32_t doubleword, uint8_t* firstByte, uint8_t* secondByte, uint8_t* thirdByte, uint8_t* fourthByte);

#endif /* BINUTILS_H_ */
