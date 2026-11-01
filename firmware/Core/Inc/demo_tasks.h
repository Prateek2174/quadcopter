/*
 * demo_tasks.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_DEMO_TASKS_H_
#define INC_DEMO_TASKS_H_

#include "app_config.h"

void vTaskSlow(void);
void vTaskFast(void);

void demo_tasks_init(void);

#endif /* INC_DEMO_TASKS_H_ */
