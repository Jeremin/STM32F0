/*
 * RecordingLight.h
 *
 *  Created on: Jul 19, 2014
 *      Author: jonasbachli
 */

#ifndef RECORDINGLIGHT_H_
#define RECORDINGLIGHT_H_


#include <stdint.h>
#include <stdbool.h>

void RecordingLight_init();
void RecordingLight_tick(uint32_t timestamp);


#endif /* RECORDINGLIGHT_H_ */
