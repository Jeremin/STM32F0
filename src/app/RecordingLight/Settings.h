/*
 * Settings.h
 *
 *  Created on: Jul 20, 2014
 *      Author: jonasbachli
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_


#include <stdint.h>
#include <stdbool.h>

typedef struct _Settings {
	uint32_t idleColor;
	uint32_t readyColor;
	uint32_t recColor;
	uint32_t cueColor;
} Settings;

void Settings_read(Settings* settings);
void Settings_write(Settings* settings);


#endif /* SETTINGS_H_ */
