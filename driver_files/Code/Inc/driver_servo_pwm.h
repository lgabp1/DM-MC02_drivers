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
typedef struct { /* Structure defining a servo PWM instance */
    TIM_HandleTypeDef* htim; // htim timer instance. Example: &htim1
    TIM_Channel channel;     // Timer channel. Example: TIM_CHANNEL_3
    float min_duty;          // Duty cycle in [0.0, 1.0] scale for minimum angle. Example: 0.025
    float max_duty;          // Duty cycle in [0.0, 1.0] scale for maximum angle. Example: 0.125
    float min_angle;         // Minimum reachable angle in degrees. Example: -135
    float max_angle;         // Maximum reachable angle. Example: 135
} servo_PWM;

/** 
 * @brief Start given servo's PWM channel
 *
 * @param servo Pointer to servo to enable
 * 
 * @return Corresponding return code of HAL_TIM_PWM_Start(...)
 */
int servo_PWM_start(servo_PWM *servo);

/** 
 * @brief Stop given servo's PWM channel
 *
 * @param servo Pointer to servo to disable
 * 
 * @return Corresponding return code of HAL_TIM_PWM_Stop(...)
 */
int servo_PWM_stop(servo_PWM *servo);

/** 
 * @brief Set the angle of given servo directly
 *
 * @param servo Pointer to servo
 * @param angle_deg Target angle (in degrees)
 * 
 * @return 1 if angle_deg is outside the servo's limit angles, 0 otherwise
 */
int servo_PWM_set_angle(servo_PWM *servo, float angle_deg);

/** 
 * @brief Go to target angle with maximum speed. This is a blocking function.
 *
 * @param servo Pointer to servo
 * @param angle_deg Target angle (in degrees)
 * @param max_speed_degps Maximum speed (in degrees/seconds)
 * 
 * @return 1 if angle_deg is outside the servo's limit angles, 0 otherwise
 */
int servo_PWM_goto_angle(servo_PWM *servo, float angle_deg, float max_speed_degps);

/** 
 * @brief Make several servos go to their respective target positions with maximum speeds. This is a blocking function.
 *
 * @param servos Pointer to servo ptr array of servos to control
 * @param angles_deg Array of respective target angles (in degrees)
 * @param max_speeds_degps Array of respective maximum speeds (in degrees/seconds)
 * @param n_servos Number of servos to drive
 * 
 * @return 1 if any angle of angles_deg is outside its servo's limit angles, 0 otherwise
 * 
 * @note All servos will start at the same time (beginning), the function will block until 
 * the last servo has reached its destination, while the other servos might have arrived earlier
 * @note servos, angles_deg and max_speeds_degps should all be arrays of size n_servos
 */
int servo_PWM_group_goto_angle(servo_PWM **servos, float *angles_deg, float *max_speeds_degps, int n_servos); // Set the angle (degrees) to go to with maximum speed (°/s), managing all given servos

#endif /* INC_DRIVER_SERVO_PWM_H_ */
