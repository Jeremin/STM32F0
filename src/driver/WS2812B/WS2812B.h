/*
 * WS2812B.h
 *
 *  Created on: Jul 23, 2014
 *      Author: jonasbachli
 */

#ifndef WS2812B_H_
#define WS2812B_H_

#include "WS2812B_config.h"
#include "Color.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct _WS2812B {
	uint32_t nbrOfLeds;
	RGB* leds;
} WS2812BLedDescription;

void WS2812B_init();

WS2812BLedDescription WS2812B_getLedDescription(void);

void WS2812B_setLedColor(RGB* color, uint32_t ledIndex);
RGB* WS2812B_getLedColor(uint32_t ledIndex);
void WS2812B_fillColor(RGB* color);
void WS2812B_fillSectionColor(RGB* color, uint32_t firstLedIndex, uint32_t lastLedIndex);

void WS2812B_setNumberOfActiveLeds(uint32_t nbrOfActiveLeds);
uint32_t WS2812B_getNumberOfActiveLeds(void);

void WS2812B_tick(uint32_t localtime);

void WS2812B_start(void);
void WS2812B_stop(void);

#endif /* WS2812B_H_ */
