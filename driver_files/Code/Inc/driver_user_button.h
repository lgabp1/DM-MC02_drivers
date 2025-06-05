/*
 * driver_user_button.h
 *
 *  Created on: May 1, 2025
 *      Author: LeoG
 */

#ifndef INC_DRIVER_USER_BUTTON_H_
#define INC_DRIVER_USER_BUTTON_H_

#include <stdint.h>
#include "main.h"
#include "driver_exti.h"

#define USER_BUTTON_INVERT_CALLBACKS // If defined (def by default), PIN RISE EDGE->press, PIN FALL EDGE->release
#define USER_BUTTON_GPIO_LETTER GPIOA
#define USER_BUTTON_GPIO_PIN GPIO_PIN_15

__weak void user_button_press_callback(); // Can be overwritten, press callback
__weak void user_button_release_callback(); // Can be overwritten, release callback
GPIO_PinState user_button_read();

#endif /* INC_DRIVER_USER_BUTTON_H_ */
