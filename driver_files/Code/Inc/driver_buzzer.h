/*
 * driver_buzzer.h
 *
 *  Created on: Apr 29, 2025
 *      Author: LeoG
 */

#ifndef INC_DRIVER_BUZZER_H_
#define INC_DRIVER_BUZZER_H_

#include "stm32h7xx_hal.h"
#include <math.h>

extern TIM_HandleTypeDef htim12;

int buzzer_enable(); // Start buzzer
int buzzer_disable(); // Stop buzzer
int buzzer_set_frequency(float frequency); // Set buzzer frequency.
int buzzer_set_midi(int midi_note_number); // Set buzzer frequency as midi not number


#endif /* INC_DRIVER_BUZZER_H_ */
