/*
 * driver_usb.h
 *
 *  Created on: May 1, 2025
 *      Author: LeoG
 */

#ifndef INC_DRIVER_USB_H_
#define INC_DRIVER_USB_H_

#include "usbd_cdc_if.h"

/* Parameters */
#define USB_BUFFER_SIZE 64 // In Bytes, maxium of 127

/* Definitions */
__weak int USB_user_callback(); // Can be overwritten, executed at the end of USB_on_receive(...
int8_t USB_transmit(uint8_t* Buf, uint16_t Len); // Send data
int8_t USB_on_receive(uint8_t* Buf, uint32_t *Len); // On receive

/*extern definitions */
extern USBD_HandleTypeDef hUsbDeviceHS;
extern uint8_t serial_buffer[USB_BUFFER_SIZE];
#endif /* INC_DRIVER_USB_H_ */
