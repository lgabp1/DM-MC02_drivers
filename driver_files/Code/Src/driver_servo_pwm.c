/*
 * driver_servo_pwm.c
 *
 *  Created on: Apr 28, 2025
 *      Author: LeoG
 */

#include "driver_servo_pwm.h"

/* Helper function: retrieves the CCR set address knowing the channel*/
volatile uint32_t *_get_CCR_pointer(servo_PWM servo) {

	switch (servo.channel) {
        case TIM_CHANNEL_1:
            return &((*servo.htim.Instance).CCR1);
        case TIM_CHANNEL_2:
            return &((*servo.htim.Instance).CCR2);
        case TIM_CHANNEL_3:
            return &((*servo.htim.Instance).CCR3);
        case TIM_CHANNEL_4:
            return &((*servo.htim.Instance).CCR4);
        case TIM_CHANNEL_5:
            return &((*servo.htim.Instance).CCR5);
        case TIM_CHANNEL_6:
            return &((*servo.htim.Instance).CCR6);
        default:
            return NULL; // Return NULL to indicate an invalid channel
    }
}

/* Start given servo's PWM
*  Returns error code.
*/
int servo_PWM_start(servo_PWM servo) {
    return HAL_TIM_PWM_Start(&servo.htim, servo.channel);
}

/* Stop given servo's PWM
*  Returns error code.
*/
int servo_PWM_stop(servo_PWM servo) {
    return HAL_TIM_PWM_Stop(&servo.htim, servo.channel);
}

/* Set given angle (in degrees) of given servo.
* Returns error code.
*/
int servo_PWM_set_angle(servo_PWM servo, float angle_deg) {
	if (angle_deg < servo.min_angle || angle_deg > servo.max_angle) {
		return 1;
	}
    int min_ccr = (servo.htim.Instance->ARR+1)*servo.min_duty*2; // Factor 2: because Timer clock is
    int max_ccr = (servo.htim.Instance->ARR+1)*servo.max_duty*2; // updated twice by APB1/2 tick
    float angle_percent = (angle_deg - servo.min_angle)/(servo.max_angle - servo.min_angle);
    int ccr = round(angle_percent*(max_ccr - min_ccr) + min_ccr);

	volatile uint32_t *ccr_ptr = _get_CCR_pointer(servo);
    (* ccr_ptr) = ccr - 1;
    return 0;
}

/* Set the angle (degrees) to go to with maximum angle speed (°/s) for given servo.
 * This is a blocking function.
 * Returns error code.
 */
int servo_PWM_goto_angle(servo_PWM servo, float angle_deg, float speed_degps) {
    if (angle_deg < servo.min_angle || angle_deg > servo.max_angle) {
        return 1;
    }

    // Get the current angle
    volatile uint32_t *ccr_ptr = _get_CCR_pointer(servo);
    int current_ccr = *ccr_ptr + 1; // Add 1 to compensate for the -1 in servo_PWM_set_angle
    int min_ccr = (servo.htim.Instance->ARR+1)*servo.min_duty*2; // Factor 2: because Timer clock is
    int max_ccr = (servo.htim.Instance->ARR+1)*servo.max_duty*2; // updated twice by APB1/2 tick
    float current_angle = servo.min_angle + (servo.max_angle - servo.min_angle) * (current_ccr - min_ccr) / (max_ccr - min_ccr);

    // Calculate the target CCR value
    float angle_percent = (angle_deg - servo.min_angle) / (servo.max_angle - servo.min_angle);
    int target_ccr = round(angle_percent * (max_ccr - min_ccr) + min_ccr);

    // Calculate the increment
    float angle_diff = angle_deg - current_angle;
    float time_ms = fabs(angle_diff) / speed_degps * 1000; // Time in milliseconds
    int steps = round(time_ms / 10); // Number of steps, assuming 10ms delay per step
    float ccr_increment = (target_ccr - current_ccr) / steps;

    // Gradually adjust the CCR value
    for (int i = 0; i < steps; i++) {
        current_ccr += ccr_increment;
        *ccr_ptr = current_ccr - 1; // Set the new CCR value
        HAL_Delay(10); // Delay for 10ms
    }

    // Ensure the final value is set correctly
    *ccr_ptr = target_ccr - 1;

    return 0;
}

/* Snippet */
/*
    uint32_t freq_source;
    if (servo.APB == APB1) {
        freq_source = HAL_RCC_GetPCLK1Freq()*2;
    } else if (servo.APB == APB2) {
        freq_source = HAL_RCC_GetPCLK2Freq()*2;
    } else {
        return 1; // error
    }
    float PWM_freq = freq_source/(servo.htim.Init.Prescaler+1);
    float PWM_period = (servo.htim.Instance->ARR+1)/PWM_freq;
    float min_ccr_t = min_time/PWM_period * (servo.htim.Instance->ARR+1);
    float max_ccr_t = max_time/PWM_period * (servo.htim.Instance->ARR+1);
*/
