/*
 * driver_exti.h
 *  A wrapper for EXTI interrupts
 *  Created on: May 2, 2025
 *      Author: LeoG
 */

#ifndef INC_DRIVER_EXTI_H_
#define INC_DRIVER_EXTI_H_

#include "main.h"

__weak void USER_GPIO_0_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO0
__weak void USER_GPIO_1_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO1
__weak void USER_GPIO_2_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO2
__weak void USER_GPIO_3_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO3
__weak void USER_GPIO_4_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO4
__weak void USER_GPIO_5_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO5
__weak void USER_GPIO_6_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO6
__weak void USER_GPIO_7_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO7
__weak void USER_GPIO_8_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO8
__weak void USER_GPIO_9_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO9
__weak void USER_GPIO_10_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO10
__weak void USER_GPIO_11_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO11
__weak void USER_GPIO_12_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO12
__weak void USER_GPIO_13_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO13
__weak void USER_GPIO_14_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO14
__weak void USER_GPIO_15_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO15
__weak void USER_GPIO_All_EXTI_CALLBACK() ; // User-defined rising/falling edge interrupt action for GPIOAll

#endif /* INC_DRIVER_EXTI_H_ */
