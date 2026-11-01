/*
 * demo_tasks.c
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */
#include "demo_tasks.h"

void vTaskSlow(void){

	TickType_t xLastWakeTime = xTaskGetTickCount();

	BaseType_t xWasDelayed = xTaskDelayUntil( &xLastWakeTime, FAST_PERIOD_TICKS);



}

void vTaskFast(void){

	TickType_t xLastWakeTime = xTaskGetTickCount();

	BaseType_t xWasDelayed = xTaskDelayUntil(&xLastWakeTime, SLOW_PERIOD_TICKS);

}

void demo_tasks_init(void){

	//TODO: creates both tasks with xTaskCreate using your priorities and
	//		stack sizes from app_config.h.
	//		Check the return value of each call.
	//		Decide what the code should do if creation fails.

	//fast task:

	 BaseType_t xFastOk = xTaskCreate(vTaskFast,
									 "fast_slow",
									 STACK_FAST_WORDS,
									 NULL,
									 PRI_TASK_FAST,
									 NULL
									 );

	 //slow task:

	 BaseType_t xSlowOk = xTaskCreate(vTaskSlow,
									 "slow_task",
									 STACK_SLOW_WORDS,
									 NULL,
									 PRI_TASK_SLOW,
									 NULL
									 );






}

