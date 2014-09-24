/*
 * BinUtils.c
 *
 *  Created on: Jul 19, 2014
 *      Author: jonasbachli
 */

#include "BinUtils.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////

uint32_t BinUtils_create32BitFrom4Bytes(uint8_t firstByte, uint8_t secondByte,
		uint8_t thirdByte, uint8_t fourthByte) {
	uint32_t result = 0;
	result = (0xff000000 & (firstByte << 24))
			| (0x00ff0000 & (secondByte << 16))
			| (0x0000ff00 & (thirdByte << 8)) | (0x000000ff & (fourthByte));
	return result;
}

void BinUtils_get4BytesFrom32Bit(uint32_t doubleword, uint8_t* firstByte,
		uint8_t* secondByte, uint8_t* thirdByte, uint8_t* fourthByte) {
	*fourthByte = (uint8_t) doubleword & 0x000000ff;
	doubleword >>= 8;
	*thirdByte = (uint8_t) doubleword & 0x000000ff;
	doubleword >>= 8;
	*secondByte = (uint8_t) doubleword & 0x000000ff;
	doubleword >>= 8;
	*firstByte = (uint8_t) doubleword & 0x000000ff;
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

