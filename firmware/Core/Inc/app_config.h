/*
 * app_config.h
 *
 *  Created on: Oct 30, 2026
 *      Author: prate
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_

#include "FreeRTOS.h"
#include "task.h"

_Static_assert(configTICK_RATE_HZ == 1000, "app_config.h assumes a 1 kHz tick");

//Periods (ms)

#define FAST_PERIOD_MS 1
#define SLOW_PERIOD_MS 500
#define MEM_PERIOD_MS 1000

//Periods (ticks)

#define FAST_PERIOD_TICKS pdMS_TO_TICKS(FAST_PERIOD_MS)
#define SLOW_PERIOD_TICKS pdMS_TO_TICKS(SLOW_PERIOD_MS)
#define MEM_PERIOD_TICKS  pdMS_TO_TICKS(MEM_PERIOD_MS)

//Priority

//Lowest Priority = 0
//Highest Priority = ( configMAX_PRIORITIES - 1 )

#define PRI_TASK_FAST (tskIDLE_PRIORITY + 3)
#define PRI_TASK_SLOW (tskIDLE_PRIORITY + 2)
#define PRI_TASK_LOG  (tskIDLE_PRIORITY + 1)
#define PRI_TASK_MEM  (tskIDLE_PRIORITY + 2)

//Stack Sizes (words) (1 word = 4 bytes)

#define STACK_FAST_WORDS 256
#define STACK_SLOW_WORDS 256
#define STACK_LOG_WORDS  512
#define STACK_MEM_WORDS  256

#endif /* INC_APP_CONFIG_H_ */
