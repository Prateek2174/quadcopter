/*
 * loop_stats.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_LOOP_STATS_H_
#define INC_LOOP_STATS_H_

#include <stdint.h>

//TODO: min, max, mean, jitter and count from one timestamp per loop.
//		Pure logic, testable on your PC.


//TODO: What does “jitter” mean in your report:
//		max minus min, or standard deviation? The mean must not overflow over many samples.

//TODO: loop_stats_reset,
//		loop_stats_record (one timestamp per loop)
//		loop_stats_get (min, max, mean, jitter)
//		and a way to know N samples are in

void loop_stats_reset(Typedef *stats); //takes in a stats object
bool loop_stats_record(Typedef *stats, uint32_t timestamp); //should timestamp be a TickType_t
void loop_stats_get();

#endif /* INC_LOOP_STATS_H_ */
