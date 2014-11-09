/*
 * DioderController.h
 *
 *  Created on: Sep 28, 2014
 *      Author: jonasbachli
 */

#ifndef DIODERCONTROLLER_H_
#define DIODERCONTROLLER_H_


#include <stdint.h>
#include <stdbool.h>

void DioderController_init(void);
void DioderController_tick(uint32_t timeInMs);

#endif /* DIODERCONTROLLER_H_ */
