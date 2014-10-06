/*
 * Color.c
 *
 *  Created on: Jul 25, 2014
 *      Author: jonasbachli
 */

#include "Color.h"
#include "TypesAndMacros.h"

/////////////////////////////////////
// forward declarations
/////////////////////////////////////

/////////////////////////////////////
// private variables
/////////////////////////////////////

/////////////////////////////////////
// public functions
/////////////////////////////////////
RGB RGBA2RGB(RGBA rgba){
	RGB rgb;
	rgb.red = (uint8_t) (((uint32_t)rgba.red)*(uint32_t)rgba.alpha)/0xff;
	rgb.green = (uint8_t) (((uint32_t)rgba.green)*(uint32_t)rgba.alpha)/0xff;
	rgb.blue = (uint8_t) (((uint32_t)rgba.blue)*(uint32_t)rgba.alpha)/0xff;
	return rgb;
}

RGBA RGB2RGBA(RGB rgb){
	RGBA rgba;
	rgba.alpha = MIN(rgb.red, MIN(rgb.green, rgb.blue));
	//!TODO
	return rgba;
}

/////////////////////////////////////
// private functions
/////////////////////////////////////

