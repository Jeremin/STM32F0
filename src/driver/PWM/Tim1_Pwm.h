/*
 * Tim1_Pwm.h
 *
 *  Created on: Aug 31, 2014
 *      Author: jonasbachli
 */

#ifndef TIM1_PWM_H_
#define TIM1_PWM_H_


#include <stdint.h>
#include <stdbool.h>

void Tim1_Pwm_init(void);

void Tim1_Pwm_setValueCh1(uint8_t value);
void Tim1_Pwm_setValueCh2(uint8_t value);
void Tim1_Pwm_setValueCh3(uint8_t value);
void Tim1_Pwm_setValueCh4(uint8_t value);


#endif /* TIM1_PWM_H_ */
