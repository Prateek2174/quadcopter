/*
 * dwt_timer.c
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#include "dwt_timer.h"
#include "stm32f4xx.h"

void dwt_init(void){

	//trace block enable   (in the core debug register)
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	//zero the counter
	DWT->CYCCNT = 0;
	//counter enable       (in the DWT control register)
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;


}

uint32_t dwt_get_cycles(void){

	//TODO: Reads the counter now.

}

uint32_t dwt_elapsed_cycles(uint32_t start, uint32_t end){

	//TODO: Computes cycles between two readings, correct across a wrap.
	return end - start;

}

float dwt_cycles_to_us(uint32_t cycle_count){

	//TODO: Converts cycles to microseconds (180 cycles per µs at 180 MHz).

}

