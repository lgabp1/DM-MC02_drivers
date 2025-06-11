/*
 * driver_servo_pwm.c
 *
 *  Created on: Apr 28, 2025
 *      Author: LeoG
 */

#include "driver_servo_pwm.h"
#include <stdlib.h>

/** Helper function
 * @brief Retrieve pointer to corresponding CCR register given the servo's channel
 *
 * @param servo Pointer to servo struct
 * @return Pointer to CCR register
 */
volatile uint32_t *_get_CCR_pointer(servo_PWM *servo) {

	switch (servo->channel) {
        case TIM_CHANNEL_1:
            return &((*servo->htim->Instance).CCR1);
        case TIM_CHANNEL_2:
            return &((*servo->htim->Instance).CCR2);
        case TIM_CHANNEL_3:
            return &((*servo->htim->Instance).CCR3);
        case TIM_CHANNEL_4:
            return &((*servo->htim->Instance).CCR4);
        case TIM_CHANNEL_5:
            return &((*servo->htim->Instance).CCR5);
        case TIM_CHANNEL_6:
            return &((*servo->htim->Instance).CCR6);
        default:
            return NULL; // Return NULL to indicate an invalid channel
    }
}

/** Helper function
 * @brief Retrieve maximum value index in uint32_t array
 *
 * @param arr Array to parse
 * @param len Array length
 * @return Index of maximum value
 */
uint32_t _get_max_ind(uint32_t *arr, uint32_t len)
{
    uint32_t maxi = 0;
    for (int i=1; i<len; i++) {
        if (arr[i] > arr[maxi]) {
            maxi = i;
        }
    }
    return maxi;
}


/** 
 * Start given servo's PWM channel
 */
int servo_PWM_start(servo_PWM *servo) {
    return HAL_TIM_PWM_Start(servo->htim, servo->channel);
}

/** 
 * Stop given servo's PWM channel
 */
int servo_PWM_stop(servo_PWM *servo) {
    return HAL_TIM_PWM_Stop(servo->htim, servo->channel);
}

/** 
 * Set the angle of given servo directly
 */
int servo_PWM_set_angle(servo_PWM *servo, float angle_deg) {
	if (angle_deg < servo->min_angle || angle_deg > servo->max_angle) {
		return 1;
	}
    uint32_t min_ccr = (servo->htim->Instance->ARR+1)*servo->min_duty*2; // Factor 2: because Timer clock is
    uint32_t max_ccr = (servo->htim->Instance->ARR+1)*servo->max_duty*2; // updated twice by APB1/2 tick
    float angle_percent = (angle_deg - servo->min_angle)/(servo->max_angle - servo->min_angle);
    uint32_t ccr = (uint32_t) round(angle_percent*(max_ccr - min_ccr) + min_ccr);

	volatile uint32_t *ccr_ptr = _get_CCR_pointer(servo);
    (* ccr_ptr) = ccr - 1;
    return 0;
}

/** 
 * Go to target angle with maximum speed. This is a blocking function.
 */
int servo_PWM_goto_angle(servo_PWM *servo, float angle_deg, float max_speed_degps) {
    if (angle_deg < servo->min_angle || angle_deg > servo->max_angle) {
        return 1;
    }

    // Get the current angle
    volatile uint32_t *ccr_ptr = _get_CCR_pointer(servo);
    uint32_t current_ccr = *ccr_ptr + 1; // Add 1 to compensate for the -1 in servo_PWM_set_angle
    uint32_t min_ccr = (servo->htim->Instance->ARR + 1) * servo->min_duty * 2; // Factor 2: because Timer clock is
    uint32_t max_ccr = (servo->htim->Instance->ARR + 1) * servo->max_duty * 2; // updated twice by APB1/2 tick
    float current_angle = servo->min_angle + (servo->max_angle - servo->min_angle) * (float)(current_ccr - min_ccr) / (max_ccr - min_ccr);

    float angle_diff = angle_deg - current_angle;
    float delay_seconds = 0.005; // 5 ms delay
    uint32_t steps = (uint32_t) round(fabs(angle_diff) / max_speed_degps / delay_seconds); // Calculate steps assuming function is called every 1ms

    uint32_t last_tick = HAL_GetTick();
    for (uint32_t i = 1; i <= steps; i++) {
        float intermediate_angle = current_angle + (angle_diff / steps) * i;
        uint32_t min_ccr = (servo->htim->Instance->ARR + 1) * servo->min_duty * 2; // Factor 2: because Timer clock is
        uint32_t max_ccr = (servo->htim->Instance->ARR + 1) * servo->max_duty * 2; // updated twice by APB1/2 tick
        float angle_percent = (intermediate_angle - servo->min_angle) / (servo->max_angle - servo->min_angle);
        uint32_t ccr = (uint32_t) round(angle_percent * (max_ccr - min_ccr) + min_ccr);

        volatile uint32_t *ccr_ptr = _get_CCR_pointer(servo);
        (*ccr_ptr) = ccr - 1;

        while (HAL_GetTick() - last_tick < 5) {
            __NOP();
        }
        last_tick = HAL_GetTick();
    }
    
    // Ensure last value is set properly
    float angle_percent = (angle_deg - servo->min_angle)/(servo->max_angle - servo->min_angle);
    uint32_t target_ccr = (uint32_t) round(angle_percent*(max_ccr - min_ccr) + min_ccr);
    *ccr_ptr = target_ccr - 1;
    return 0;
}

/** 
 * Make several servos go to their respective target positions with maximum speeds. This is a blocking function.
 */
int servo_PWM_group_goto_angle(servo_PWM **servos, float *angles_deg, float *max_speeds_degps, int n_servos) {
    uint32_t i; uint32_t k;
    for (k=0; k<n_servos; k++) {
        if (angles_deg[k] < servos[k]->min_angle || angles_deg[k] > servos[k]->max_angle)
        {
            return 1;
        }
    }

    uint32_t current_ccr; uint32_t min_ccr; uint32_t max_ccr;
    uint32_t *servo_steps; servo_steps = (uint32_t *) malloc(n_servos * sizeof(uint32_t));
    float *current_angles; current_angles = (float *) malloc(n_servos * sizeof(float));
    float *angle_diffs; angle_diffs = (float *) malloc(n_servos * sizeof(float));
    if (servo_steps == NULL || current_angles == NULL || angle_diffs == NULL) {return 1;}

    // Get the steps angles
    for (k=0; k<n_servos; k++) {
        volatile uint32_t *ccr_ptr = _get_CCR_pointer(servos[k]);
        current_ccr = *ccr_ptr + 1; // Add 1 to compensate for the -1 in servo_PWM_set_angle
        min_ccr = (servos[k]->htim->Instance->ARR + 1) * servos[k]->min_duty * 2;
        max_ccr = (servos[k]->htim->Instance->ARR + 1) * servos[k]->max_duty * 2;
        current_angles[k] = servos[k]->min_angle + (servos[k]->max_angle - servos[k]->min_angle) * (float)(current_ccr - min_ccr) / (max_ccr - min_ccr);

        angle_diffs[k] = angles_deg[k] - current_angles[k];
        double delay_seconds = 0.005; // 5 ms delay
        servo_steps[k] = (uint32_t) round( (fabs(angle_diffs[k]) / max_speeds_degps[k] / delay_seconds) );; // Calculate steps assuming function is called every 1ms
    }

    // Drive the servos
    uint32_t max_steps = servo_steps[_get_max_ind(servo_steps, n_servos)];
    uint32_t last_tick = HAL_GetTick();
    for (i = 1; i <= max_steps; i++) {
        for (k=0; k<n_servos; k++) {
            if (i > servo_steps[k]) {continue;} // skip servos that have reached destination

            float intermediate_angle = current_angles[k] + (angle_diffs[k] / servo_steps[k]) * i;
            uint32_t min_ccr = (servos[k]->htim->Instance->ARR + 1) * servos[k]->min_duty * 2;
            uint32_t max_ccr = (servos[k]->htim->Instance->ARR + 1) * servos[k]->max_duty * 2;
            float angle_percent = (intermediate_angle - servos[k]->min_angle) / (servos[k]->max_angle - servos[k]->min_angle);
            uint32_t ccr = (uint32_t) round(angle_percent * (max_ccr - min_ccr) + min_ccr);

            volatile uint32_t *ccr_ptr = _get_CCR_pointer(servos[k]);
            (*ccr_ptr) = ccr - 1;

		while (HAL_GetTick() - last_tick < 5) {
			__NOP();
		}
		last_tick = HAL_GetTick();
        }
    }

    // Ensure last value is set properly
    for (k=0; k<n_servos; k++) {
        volatile uint32_t *ccr_ptr = _get_CCR_pointer(servos[k]);
        float angle_percent = (angles_deg[k] - servos[k]->min_angle) / (servos[k]->max_angle - servos[k]->min_angle);
        uint32_t target_ccr = (uint32_t) round(angle_percent * (max_ccr - min_ccr) + min_ccr);
        *ccr_ptr = target_ccr - 1;
    }
    return 0;
}
