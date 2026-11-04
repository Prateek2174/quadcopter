/*
 * timing_pin.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_TIMING_PIN_H_
#define INC_TIMING_PIN_H_

//TODO: set and clear the timing GPIO as cheaply as possible.

void timing_pin_high(void);
void timing_pin_low(void);
void timing_pin_toggle(void);

#endif /* INC_TIMING_PIN_H_ */
