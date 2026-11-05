/*
 * mem_monitor.c
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#include "mem_monitor.h"
#include "app_config.h"
#include "FreeRTOS.h"
#include "task.h"

// 1 word = 4 bytes

#define MAX_HANDLES 15

//create a struct array to hold all the task handles so it can hold it for all tasks

typedef struct {

	const char *name;
	TaskHandle_t handle;

} mem_monitor_handle_t;

static mem_monitor_handle_t handle_table[MAX_HANDLES];
static uint32_t handle_count; //to keep count of current handles in the table

static volatile uint32_t high_water_mark;

uint32_t mem_monitor_get_high_water_mark(void){

	return high_water_mark;

}

bool mem_monitor_register(const char *name, TaskHandle_t handle){

	if(handle_count >= MAX_HANDLES){
		return false; //table is full
	}

	handle_table[handle_count].name = name;
	handle_table[handle_count].handle = handle;
	handle_count++;
	return true;

}


static void mem_monitor_task(void * pv_parameters){

	TickType_t last_wake_time = xTaskGetTickCount();



	for(;;){

		// Wait for the next cycle.
		vTaskDelayUntil(&last_wake_time, MEM_PERIOD_TICKS); //wake once per second

		//Work:

		high_water_mark = uxTaskGetStackHighWaterMark(NULL);



	}

}

void mem_monitor_init(void){

	TaskHandle_t mem_task_handle;

	BaseType_t mem_task_ok = xTaskCreate(mem_monitor_task,
									 "mem_mon_task",
									 STACK_MEM_WORDS,
									 NULL,
									 PRI_TASK_MEM,
									 &mem_task_handle
									 );

	mem_monitor_register("mem_mon_task", mem_task_handle);


	 configASSERT(mem_task_ok == pdPASS);


}
