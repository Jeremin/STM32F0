/*
 * Color.h
 *
 *  Created on: Jul 19, 2014
 *      Author: jonasbachli
 */

#ifndef COLOR_H_
#define COLOR_H_


#include <stdint.h>
#include <stdbool.h>

typedef struct _RGB{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} RGB;

typedef struct _RGBA{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} RGBA;

RGB RGBA2RGB(RGBA rgba);
RGBA RGB2RGBA(RGB rgb);

#endif /* COLOR_H_ */
