/*
 * loop_stats.c
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#include "loop_stats.h"

void loop_stats_reset(loop_stats_t *stats){

	stats -> prev_exists = false;
	stats -> prev_timestamp = 0;
	stats -> min = 0xFFFFFFFF;
	stats -> max = 0;
	stats -> sum = 0;
	stats -> count = 0;

}

bool loop_stats_record(loop_stats_t *stats, uint32_t timestamp){

	uint32_t period;

	if (stats -> prev_exists){

		period = timestamp - stats -> prev_timestamp;

		if(period > stats -> max) stats -> max = period;
		if(period < stats -> min) stats -> min = period;

		stats -> prev_timestamp = timestamp;
		stats -> sum += period;
		stats -> count++;

		return true;

	} else {

		stats -> prev_timestamp = timestamp;
		stats -> prev_exists = true;
		return false;
	}

}

bool loop_stats_get(loop_stats_t *stats, loop_stats_result_t *result){

	if (stats -> count >= 1){

		result -> min = stats -> min;
		result -> max = stats -> max;
		result -> mean = stats -> sum / stats -> count;
		result -> jitter = stats -> max - stats -> min;
		return true;

	}

	return false;

}

