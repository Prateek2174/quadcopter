/*
 * runtime_stats.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_RUNTIME_STATS_H_
#define INC_RUNTIME_STATS_H_

#include <stdint.h>

//TODO: run-time stats timebase hooks, and a CPU-load table printer.

//TODO: Which timer, and what happens when it wraps?

//TODO: runtime_stats_timer_init
//		runtime_stats_get_counter
//		runtime_stats_print

void runtime_stats_timer_init(void);
uint32_t runtime_stats_get_counter(void);


#endif /* INC_RUNTIME_STATS_H_ */
