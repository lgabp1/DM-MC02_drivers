/*
 * driver_buzzer.c
 *
 *  Created on: Jun 09, 2025
 *      Author: LeoG
 */

#include "driver_buzzer.h"
volatile float _BUZZER_INTENSITY = 0.5; // duty cycle in [0.0, 1.0] scale.

/*
 * Start the buzzer by starting its related timer timer
 */
int buzzer_enable() {
	return (int)HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
}

/*
 * Stops the buzzer by stopping its related timer
 */
int buzzer_disable() {
	return (int)HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_2);
}

/*
 * Set buzzer frequency
 */
int buzzer_set_frequency(float frequency) {
	uint32_t freq_apb1 = HAL_RCC_GetPCLK1Freq();
	// fb = fc/p => p = fc/fb
	uint32_t prescaler = round(freq_apb1/frequency);
	/* Set prescaler */
	TIM12->ARR = prescaler - 1;
	TIM12->CCR2 = round(prescaler * _BUZZER_INTENSITY) - 1;
	return 0;
}

/*
 * Set buzzer frequency as midi note.
 */
int buzzer_set_midi(int midi_note_number) {
	float midi_note_freq = 440.0 * pow(2.0, (midi_note_number - 69)/(12.0) );
	return buzzer_set_frequency(midi_note_freq);
}
