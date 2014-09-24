/*
 * Settings.c
 *
 *  Created on: Jul 20, 2014
 *      Author: jonasbachli
 */

#include "Settings.h"
#include "Flash.h"

#include <string.h>

#define SETTINGS_HEADER_VALUE				((uint32_t)0x5265634C)	//RecL

#define SETTINGS_DEFAULT_VALUE_IDLE_COLOR	0xafafafff
#define SETTINGS_DEFAULT_VALUE_READY_COLOR	0xddff00ff
#define SETTINGS_DEFAULT_VALUE_REC_COLOR	0xff0000ff
#define SETTINGS_DEFAULT_VALUE_CUE_COLOR	0x0000ffff

typedef struct {
	uint32_t header;
	Settings settings;
} SettingsMemoryContainer;

/////////////////////////////////////
// forward declarations
/////////////////////////////////////
static void init(void);
/////////////////////////////////////
// private variables
/////////////////////////////////////
bool isInited = false;
/////////////////////////////////////
// public functions
/////////////////////////////////////

void Settings_read(Settings* settings){
	SettingsMemoryContainer memSettings;

	if (!isInited){
		init();
	}

	Flash_readFromDataPage(&memSettings, sizeof(SettingsMemoryContainer));

	if (memSettings.header == SETTINGS_HEADER_VALUE){
		memcpy((void*)settings, (void*) &memSettings.settings, sizeof(Settings));
	}
	else {
		settings->idleColor = SETTINGS_DEFAULT_VALUE_IDLE_COLOR;
		settings->readyColor = SETTINGS_DEFAULT_VALUE_READY_COLOR;
		settings->recColor = SETTINGS_DEFAULT_VALUE_REC_COLOR;
		settings->cueColor = SETTINGS_DEFAULT_VALUE_CUE_COLOR;
	}
}

void Settings_write(Settings* settings){
	SettingsMemoryContainer memSettings;

	if (!isInited){
			init();
	}

	memcpy((void*) &memSettings.settings, (void*) settings, sizeof(Settings));
	memSettings.header = SETTINGS_HEADER_VALUE;

	Flash_writeToDataPage(&memSettings, sizeof(SettingsMemoryContainer));
}
/////////////////////////////////////
// private functions
/////////////////////////////////////
static void init(void){
	isInited = true;
	Flash_init();
}
