/*
 * inversion_demo.h
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#ifndef INC_INVERSION_DEMO_H_
#define INC_INVERSION_DEMO_H_

//TODO: low, medium and high tasks sharing a resource,
//		first with a semaphore and then with a mutex.

//TODO: How do you switch between the semaphore and mutex versions, and what do you timestamp?

//TODO: task_low,
//		task_medium,
//		task_high
//		inversion_demo_init

void inversion_demo_init(void);
void task_low(void);
void task_medium(void);
void task_high(void);

#endif /* INC_INVERSION_DEMO_H_ */
