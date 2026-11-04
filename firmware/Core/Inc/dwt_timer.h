/*
 * dwt_timer.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_DWT_TIMER_H_
#define INC_DWT_TIMER_H_

#include <stdint.h>

//TODO: enable the cycle counter, read it, convert cycles to µs, handle 32-bit wrap.

//TODO: dwt_elapsed_cycles must stay correct when the 32-bit counter wraps.

//TODO: dwt_init
//		dwt_get_cycles
//		dwt_elapsed_cycles
//		dwt_cycles_to_us

void dwt_init(void);
uint32_t dwt_get_cycles(void);
uint32_t dwt_elapsed_cycles(uint32_t start, uint32_t end); //should take in 2 readings?
float dwt_cycles_to_us(uint32_t cycle_count);


#endif /* INC_DWT_TIMER_H_ */
