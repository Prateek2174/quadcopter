/*
 * loop_stats.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_LOOP_STATS_H_
#define INC_LOOP_STATS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct{

	bool prev_exists;
	uint32_t prev_timestamp;
	uint32_t min;
	uint32_t max;
	uint64_t sum;
	uint32_t count;

}loop_stats_t;

typedef struct{

	uint32_t min;
	uint32_t max;
	uint64_t mean;
	uint32_t jitter;

}loop_stats_result_t;

//TODO: loop_stats_reset,
//		loop_stats_record (one timestamp per loop)
//		loop_stats_get (min, max, mean, jitter)
//		and a way to know N samples are in

void loop_stats_reset(loop_stats_t *stats); //takes in a stats object
bool loop_stats_record(loop_stats_t *stats, uint32_t timestamp); //TRUE = produced a period
bool loop_stats_get(loop_stats_t *stats, loop_stats_result_t *result);

#endif /* INC_LOOP_STATS_H_ */
