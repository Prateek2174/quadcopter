/*
 * app_log.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_APP_LOG_H_
#define INC_APP_LOG_H_

//TODO: a queue plus one low-priority task that owns the USART2 TX.

//TODO: What should app_log_write do when the queue is full:
//		wait, drop, or count drops?

//TODO: app_log_init (creates the queue and the task),
//		app_log_write,
//		the log task body

void app_log_init(void);


#endif /* INC_APP_LOG_H_ */
