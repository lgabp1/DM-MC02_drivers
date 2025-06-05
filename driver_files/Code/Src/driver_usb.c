/*
 * driver_usb.c
 *
 *  Created on: May 1, 2025
 *      Author: LeoG
 */
#include "driver_usb.h"
uint8_t serial_buffer[USB_BUFFER_SIZE];

/*
 * Function called upon serial data reception.
 */
int8_t USB_on_receive(uint8_t* Buf, uint32_t *Len) // On receive
{
	/* Code inpired by this (wonderful) guide: https://controllerstech.com/send-and-receive-data-to-pc-without-uart-stm32-usb-com/ */
    USBD_CDC_SetRxBuffer(&hUsbDeviceHS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceHS);

    memset(serial_buffer, '\0', USB_BUFFER_SIZE);  // Clear the buffer

    memcpy(serial_buffer, Buf, *Len);  // Copy the data to the buffer
    memset(Buf, '\0', *Len);    // Clear the Buf also

	int8_t ret = (int8_t)USB_user_callback(); // Call user-defined callback (empty if not defined)
	if (ret) {return ret;}

    return (USBD_OK);
}

/*
 * Send data over serial.
 */
int8_t USB_transmit(uint8_t* Buf, uint16_t Len) {
	  return (int8_t)CDC_Transmit_HS(Buf, Len);
}

