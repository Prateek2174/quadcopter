/*
 * mem_monitor.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_MEM_MONITOR_H_
#define INC_MEM_MONITOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"



//TODO: stack high-water-mark table, plus the stack-overflow and malloc-failed hooks.

//TODO: What do the hooks do?
//		They run in a failing system, so they can’t depend on the logger working.

//TODO: mem_monitor_report (per-task stack headroom),
//		mem_monitor_heap_report (free heap now and the minimum ever),
//		the two hooks

void mem_monitor_init(void);
bool mem_monitor_register(const char *name, TaskHandle_t handle);
uint32_t mem_monitor_get_high_water_mark(void);
// mem_monitor_report(void); //a printed table -> what return type?
// mem_monitor_heap_report(void); //return needs to be 2 numbers so array?


#endif /* INC_MEM_MONITOR_H_ */
