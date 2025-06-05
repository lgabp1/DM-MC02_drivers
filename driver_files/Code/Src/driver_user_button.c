/*
 * driver_user_button.c
 *
 *  Created on: May 1, 2025
 *      Author: LeoG
 */

#include "driver_user_button.h"

GPIO_PinState user_button_read() {

	int a = HAL_GPIO_ReadPin(USER_BUTTON_GPIO_LETTER, USER_BUTTON_GPIO_PIN);
	return (GPIO_PinState) a;
}


/*
 * Callback to interface with the GPIO interrupt, using the driver_exti.h wrapper
 */
void USER_GPIO_15_EXTI_CALLBACK()
{
#ifdef USER_BUTTON_INVERT_CALLBACKS // Default behaviour
  if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_LETTER, USER_BUTTON_GPIO_PIN)) {
	  user_button_release_callback();
  } else {
	  user_button_press_callback();
  }
#else
  if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_LETTER, USER_BUTTON_GPIO_PIN)) {
	  user_button_press_callback();
  } else {
	  user_button_release_callback();
  }
#endif
}
