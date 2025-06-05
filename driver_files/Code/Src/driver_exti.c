/*
 * driver_exit.c
 *
 *  Created on: May 2, 2025
 *      Author: LeoG
 */

#include "driver_exti.h"

/*
 * Override HAL_GPIO_EXTI_Callback, called when a GPIO with interrupt activated changes.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
	    case GPIO_PIN_0   : USER_GPIO_0_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO0
	    case GPIO_PIN_1   : USER_GPIO_1_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO1
	    case GPIO_PIN_2   : USER_GPIO_2_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO2
	    case GPIO_PIN_3   : USER_GPIO_3_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO3
	    case GPIO_PIN_4   : USER_GPIO_4_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO4
	    case GPIO_PIN_5   : USER_GPIO_5_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO5
	    case GPIO_PIN_6   : USER_GPIO_6_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO6
	    case GPIO_PIN_7   : USER_GPIO_7_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO7
	    case GPIO_PIN_8   : USER_GPIO_8_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO8
	    case GPIO_PIN_9   : USER_GPIO_9_EXTI_CALLBACK()   ; // User-defined rising/falling edge interrupt action for GPIO9
	    case GPIO_PIN_10  : USER_GPIO_10_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO10
	    case GPIO_PIN_11  : USER_GPIO_11_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO11
	    case GPIO_PIN_12  : USER_GPIO_12_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO12
	    case GPIO_PIN_13  : USER_GPIO_13_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO13
	    case GPIO_PIN_14  : USER_GPIO_14_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO14
	    case GPIO_PIN_15  : USER_GPIO_15_EXTI_CALLBACK()  ; // User-defined rising/falling edge interrupt action for GPIO15
	    case GPIO_PIN_All : USER_GPIO_All_EXTI_CALLBACK() ; // User-defined rising/falling edge interrupt action for GPIOAll
	    default: __NOP();
	}
}



