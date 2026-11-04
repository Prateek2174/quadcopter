/*
 * timing_pin.c
 *
 *  Created on: Oct 31, 2026
 *      Author: prate
 */

#include "timing_pin.h"
#include "main.h"

#define TIMING_PORT LD2_GPIO_Port
#define TIMING_PIN LD2_Pin

void timing_pin_high(void){

	HAL_GPIO_WritePin(TIMING_PORT, TIMING_PIN, GPIO_PIN_SET);

}

void timing_pin_low(void){

	HAL_GPIO_WritePin(TIMING_PORT, TIMING_PIN, GPIO_PIN_RESET);

}

void timing_pin_toggle(void){

	HAL_GPIO_TogglePin(TIMING_PORT, TIMING_PIN);

}
