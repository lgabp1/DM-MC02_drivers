# DM-MC02 drivers

Some drivers written for [DM-MC02 Board](https://gitee.com/kit-miao/dm-mc02) to encapsulate the board's features.

## Requirements
This library was made for the STM32CubeIDE environment, and designed for the [DM-MC02 Board](https://gitee.com/kit-miao/dm-mc02) based on [STM32h723vgT6](https://www.st.com/en/microcontrollers-microprocessors/stm32h723vg.html) MCU.

## Repository content
This repository provides several resources:
* [`STM32CubeIDE_project`](./STM32CubeIDE_project/) contains an example STM32CubeIDE project
* [`driver_files`](./driver_files/) contains the driver's source code
A README.md file can be found in each of these folders to provide additionnal information.

Please also refer to [DriverUsage.md](./DriverUsage.md) for more information on how to use the drivers.

## Features
This repository implement the following librairies:
* `driver_exti`: A wrapper to handle GPIO external interrupts
* `driver_servo_pwm`: Drive any PWM-controlled servo motors
* `driver_buzzer`: Control the DM-MC02's built-in buzzer
* `driver_usb`: Wrapper to handle serial USB communication
* `driver_user_button`: Handle the DM-MC02's user button

Additionnal instructions are also given to
* use the manufacturer's `ws2812` library to control the DM-MC02's adressable LED
* handle FDCAN communication

## Known issues
* Issue with `ws2812` library causing wrong colors to be displayed. Probably due to timing issues.

## Contributing

I would be happy to receive any contributions to this project. Please also feel free to fork and use the code as you see fit.

## License

Not yet decided as this repository shares STM32CubeIDE projects.

## Acknowledgements

Example code from the DM-MC02's manufacturer [here](https://gitee.com/kit-miao/dm-mc02/tree/master) greatly helped the driver's development.

## Version history
* `v1.00` Initial release
