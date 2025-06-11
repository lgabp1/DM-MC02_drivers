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

extern TIM_HandleTypeDef htim12; // htim object driving the buzzer

/**
 * @brief Start the buzzer by starting its related timer timer
 *
 * @return Returns 0 on success.
 */
int buzzer_enable();

/**
 * @brief Stops the buzzer by stopping its related timer
 *
 * @return Returns 0 on success.
 */
int buzzer_disable();

/**
 * @brief Set buzzer frequency
 *
 * @param frequency The desired frequency of the buzzer in Hz.
 * @return Returns 0 on success.
 */
int buzzer_set_frequency(float frequency);

/**
 * @brief Set buzzer frequency as midi note.
 *
 * @param midi_note_number The desired midi note to play on the buzzer.
 * @return Returns 0 on success.
 * 
 * @note Because of the buzzer's limited oscillating frequency range, midi notes under 69 will not be heard.
 */
int buzzer_set_midi(int midi_note_number);


#endif /* INC_DRIVER_BUZZER_H_ */
