/*
 * demo_tasks.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_DEMO_TASKS_H_
#define INC_DEMO_TASKS_H_

#include <stdint.h>


void demo_tasks_init(void);
uint32_t demo_tasks_get_fast_err(void);
uint32_t demo_tasks_get_slow_err(void);
uint32_t demo_tasks_get_fast_max_late(void);
uint32_t demo_tasks_get_slow_max_late(void);

#endif /* INC_DEMO_TASKS_H_ */
