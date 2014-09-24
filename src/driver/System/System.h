/*
 * System.h
 *
 *  Created on: Jul 11, 2014
 *      Author: jonasbachli
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdint.h>
#include <stdbool.h>

typedef void (*taskCallbackFct)(uint32_t time);

void System_init(void);

void System_PITSetup(uint32_t systickInMs);
void System_PITSetTaskCallback(taskCallbackFct taskCB);
void System_PITSetIdleCallback(taskCallbackFct idleCB);
void System_PITStart(void);

#endif /* SYSTEM_H_ */
