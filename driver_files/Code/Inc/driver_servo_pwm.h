/*
 * driver_servo_pwm.h
 *
 *  Created on: Apr 28, 2025
 *      Author: LeoG
 *
 *  Driver for handling servo motors supporting PWM.
 *	Please enter the relevant parameters under the User Settings section of this file.
 */

#ifndef INC_DRIVER_SERVO_PWM_H_
#define INC_DRIVER_SERVO_PWM_H_

/* === Includes === */
#include "stm32h7xx_hal.h"
//#include "stm32h7xx_hal_conf.h"
#include <math.h>

/* Defition of a servo, its relevant pin, timer and parameters */
typedef uint32_t TIM_Channel;
typedef struct {
    TIM_HandleTypeDef htim; // htim timer instance. Example: htim1
    TIM_Channel channel; // Timer channel. Example: TIM_CHANNEL_3
    float min_duty; // Duty cycle in [0.0, 1.0] scale for minimum angle. Example: 0.025
    float max_duty; // Duty cycle in [0.0, 1.0] scale for maximum angle. Example: 0.125
    float min_angle; // Minimum reachable angle in degrees. Example: -135
    float max_angle; // Maximum reachable angle. Example: 135
} servo_PWM;


volatile uint32_t *_get_CCR_pointer(servo_PWM servo); // retrieve a pointer to the right CCR register given the servo's channel
int servo_PWM_start(servo_PWM servo); // Start given motor
int servo_PWM_stop(servo_PWM servo); // Stop given motor
int servo_PWM_set_angle(servo_PWM servo, float angle_deg); // Set directly the angle (degrees) to go to
int servo_PWM_goto_angle(servo_PWM servo, float angle_deg, float speed_degps); // Set the angle (degrees) to go to with maximum speed (°/s)

#endif /* INC_DRIVER_SERVO_PWM_H_ */
