/*
 * demo_tasks.c
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */
#include "demo_tasks.h"
#include "app_config.h"
#include "mem_monitor.h"

static volatile uint32_t slow_err_count;
static volatile uint32_t fast_err_count;
static volatile uint32_t slow_max_lateness;
static volatile uint32_t fast_max_lateness;

uint32_t demo_tasks_get_fast_err(void){
	return fast_err_count;
}

uint32_t demo_tasks_get_slow_err(void){
	return slow_err_count;
}

uint32_t demo_tasks_get_fast_max_late(void){
	return fast_max_lateness;
}

uint32_t demo_tasks_get_slow_max_late(void){
	return slow_max_lateness;
}

static void slow_task(void * pv_parameters){

	TickType_t last_wake_time = xTaskGetTickCount();
	TickType_t lateness;

	//start work:

	for(;;){

		// Wait for the next cycle.
		vTaskDelayUntil(&last_wake_time, SLOW_PERIOD_TICKS);

		lateness = xTaskGetTickCount() - last_wake_time;
		// Perform action here. was_delayed value can be used to determine
		// whether a deadline was missed if the code here took too long.

		if(lateness > 0){

			//TODO: count it & record how late it was

			slow_err_count++;

			if(lateness > slow_max_lateness){

				slow_max_lateness = lateness;

			}

		}



	}

}

static void fast_task(void * pv_parameters){

	TickType_t last_wake_time = xTaskGetTickCount();
	TickType_t lateness;


	for(;;){

		// Wait for the next cycle.
		vTaskDelayUntil(&last_wake_time, FAST_PERIOD_TICKS);

		lateness = xTaskGetTickCount() - last_wake_time;

		// Perform action here. was_delayed value can be used to determine
		// whether a deadline was missed if the code here took too long.

		//start work:


		//end work

		if(lateness > 0){

			//TODO: if not delayed check
			fast_err_count++;

			if(lateness > fast_max_lateness){

				fast_max_lateness = lateness;

			}

		}

	}

}

void demo_tasks_init(void){

	TaskHandle_t fast_handle;
	TaskHandle_t slow_handle;

	//fast task:

	 BaseType_t fast_task_ok = xTaskCreate(fast_task,
									 "fast_task",
									 STACK_FAST_WORDS,
									 NULL,
									 PRI_TASK_FAST,
									 &fast_handle
									 );

	 //slow task:

	 BaseType_t slow_task_ok = xTaskCreate(slow_task,
									 "slow_task",
									 STACK_SLOW_WORDS,
									 NULL,
									 PRI_TASK_SLOW,
									 &slow_handle
									 );

	 configASSERT(fast_task_ok == pdPASS);
	 mem_monitor_register("fast_task", fast_handle); //register the task handle for mem_monitoring

		 //disables interrupts and spins forever,
		 //which parks the CPU at a place a debugger can find

	 configASSERT(slow_task_ok == pdPASS);
	 mem_monitor_register("slow_task", slow_handle);


}

