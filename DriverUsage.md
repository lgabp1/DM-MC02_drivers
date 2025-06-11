# About the drivers

This document will briefely explain the custom made drivers for the DM-MC02 board.

# Table of Content

- [Drivers](#drivers)
    - [General setup](#general-setup)
    - [EXTI wrapper](#exti-wrapper-driver_extich)
    - [Servo PWM](#servo-pwm-driver_servo_pwmch)
    - [Buzzer](#buzzer-driver_buzzerch)
    - [USB serial](#usb-serial-driver_usb_serialch)
    - [User button](#user-button-driver_user_buttonhc)
    - [FDCAN / CAN](#fdcan--can)
    - [Adressable LED WS2812](#adressable-led-ws2812-ws2812ch)
- [Driver usage](#driver-usage)
    - [EXTI wrapper usage](#exti-wrapper-usage)
    - [Servo PWM usage](#servo-pwm-usage)
    - [Buzzer usage](#buzzer-usage)
    - [USB serial usage](#usb-serial-usage)
    - [User button usage](#user-button-usage)
    - [FDCAN / CAN usage](#fdcan--can-usage)
    - [Adressable LED WS2812 usage](#adressable-led-ws2812-usage)

# Drivers

## General setup

General setup required for several drivers.

### 1. DEBUG

**Required pinout:**

To allow code upload and debugging using an ST-Link USB adapter, the following Pin should be configured:
- Trace and Debug/DEBUG: Debug = Serial Wire
- PC15 -> POWER_OUT5V (allow 5V output on the pins)

To allow power output for the timer pins, the following configuration should be done:
- Set pin PC15 (PC15-OSC32_OUT) as GPIO_OUTPUT
- In System Core/GPIO:
    - GPIO output level = High
    - GPIO mode = Output Push Pull
    - GPIO Pull-up/Pull-down = No pull-up and no pull-down
    - User Label = POWER_OUT_5V

### 2. High Speed Clock

Several drivers make use of the high frequency crystal oscillator from the board.

**Required pinout:**

- System Core/RCC:
    * High Speed Clock (HSE) = Crystal/Ceramic Resonator

**Clock config:**

![high_speed_clock_image](https://github.com/user-attachments/assets/0111e3eb-f7b0-4f27-8fea-57568a40f945)
Input frequency should be $24MHz$ (freq of the crystal), and it should be selected as shown on the figure above.

## EXTI wrapper (driver_exti.c/h)

*files* : `driver_exti.h` , `driver_exti.c`

Wrapper for `void HAL_GPIO_EXTI_Callback(...)` to handle GPIO interrupts, allowing definition of interrupt behaviours from several libraries.

(No pinout or clock config required)

**TODO**: Allow for other GPIO letters ? (PB.. PC.. PD..) ? Or perhaps this is not how it works ?


## Servo PWM (driver_servo_pwm.c/h)

*files* : `driver_servo_pwm.h` , `driver_servo_pwm.c`

Drive up to 4 PWM-driven servo motors.

**Required pinout and clock config:**

![timer_setup](https://github.com/user-attachments/assets/4f571a03-8650-4abf-90c2-69893f6577a7)
The timers should be set accordingly, they will define the frequency $f_{PWM}$ and duty cycle $d_{c}$. The above figure shows to which busses each timer is linked to, while to which timer and channel each pin is attached to.

We will define for each PIN $i$:
* $f_{b_i}$ the "base clock" frequency
* $f_{PWM_i}$ the target PWM frequency that is required for that servomotor
* $ARR$ : count of elementary subdivisions for a PWM period (discretized time) 
This allows defining:
* $P_{i} = \frac{f_{b_i}}{f_{PWM_i}} × \frac{1}{ARR}$ the `Prescaler`
Sometimes, a range of PWM frequencies is supported by the servomotor, in such a case one may pick any frequency in this range as $f_{PWM_i}$.

Thus, the following configurations should be done (*Example values*):
- Pin1 (PE13, TIM1_CH3)
   * Take note of nominal PWM period / frequency for the 1st servo motor: $f_{PWM_1}$ (*20ms ⇔ 50Hz*)
   * In `Clock Configuration`, take note of `To APB2 Peripheral Clocks`: $f_{b_1}$ (*$120.10^6 Hz$*) 
   * Timers/TIM1:
       * Clock Source = Internal Clock
       * Channel3 = PWM Generation CH3
           * Prescaler = $P_1 - 1$ (*120*2 - 1*)
           * Counter Period (AutoReload Register, ARR) = $ARR - 1$ (*10000 - 1*)
- Pin2 (PE09, TIM1_CH1)
   * Take note of nominal PWM period / frequency for the 2nd servo motor: $f_{PWM_2}$ (*20ms ⇔ 50Hz*)
   * In `Clock Configuration`, take note of `To APB2 Peripheral Clocks`: $f_{b_1}$ (*$120.10^6 Hz$*) 
   * Timers/TIM1:
       * Clock Source = Internal Clock
       * Channel1 = PWM Generation CH1
           * Prescaler = $P_1 - 1$ (*120*2 - 1*)
           * Counter Period (AutoReload Register, ARR) = $ARR - 1$ (*10000 - 1*)
- Pin3 (PE02, TIM2_CH3)
   * Take note of nominal PWM period / frequency for the 3rd servo motor: $f_{PWM_3}$ (*20ms ⇔ 50Hz*)
   * In `Clock Configuration`, take note of `To APB2 Peripheral Clocks`: $f_{b_1}$ (*$120.10^6 Hz$*) 
   * Timers/TIM1:
       * Clock Source = Internal Clock
       * Channel2 = PWM Generation CH3
           * Prescaler = $P_1 - 1$ (*120*2 - 1*)
           * Counter Period (AutoReload Register, ARR) = $ARR - 1$ (*10000 - 1*)
- Pin4 (PE00, TIM2_CH1)
   * Take note of nominal PWM period / frequency for the 4th servo motor: $f_{PWM_4}$ (*20ms ⇔ 50Hz*)
   * In `Clock Configuration`, take note of `To APB2 Peripheral Clocks`: $f_{b_1}$ (*$120.10^6 Hz$*) 
   * Timers/TIM1:
       * Clock Source = Internal Clock
       * Channel2 = PWM Generation CH1
           * Prescaler = $P_1 - 1$ (*120*2 - 1*)
           * Counter Period (AutoReload Register, ARR) = $ARR - 1$ (*10000 - 1*)

**Notes**
For the Yeahbot TBS-K20 stepmotor:
* 270° of amplitude, 0.3° accuracy: need for 900 subdivisions of the 2ms maxiumum amplitude: needs at least 9000 subdivisions of the 20ms PWM signal period to reach this accuracy.


## Buzzer (driver_buzzer.c/h)
*files* : `driver_buzzer` , `driver_buzzer`

**Pinout:**
- Timers/TIM12:
    * Internal clock: ENABLE
    * Channel2: PWM Generation CH2

**Clock config**
TIM12 is connected to APB1. In order to achieve a frequency range of 100Hz - 10kHz with minimum of 5Hz precision, a reference clock frequency of 120MHz is enough (maximum frequency under which the frequencies have at most 5Hz precision higher than 24KHz if APB1 is set at 120MHz). 

Therefore, APB1 set at 120MHz is plenty enough, and matches the requirements of the other clocks described in this document.

However, please note that due to the buzzer's limited excitation frequency range, not all frequencies are available (at least a few KHz).

## USB serial (driver_usb.c/h)
*files* : `driver_usb.h` , `driver_usb.c`

Driver to handle USB serial communication.
Code base vastly inspired by the following guide: https://controllerstech.com/send-and-receive-data-to-pc-without-uart-stm32-usb-com/

**Pinout:**
- Connectivity/USB_OTG_HS:
    * Internal FS Phy = Device_Only
- Middleware and Software Packs/USB_DEVICE:
    * Class for HS IP = Communication Device Class (Virtual Port Com)

**Clock config**
In USB Clock Mux, select HSI48 as clock source (which should be at 48MHz) for To USB.

Note: this will allow communication through USB serial mode, at baudrate 115200.

**Source file modifications and setup**
To use this wrapper library, the file `./USB_DEVICE/APP/usbd_cdc_if.c` should be modified slightly.

*New definitions:*
```c
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
extern int8_t USB_on_receive(uint8_t* Buf, uint32_t *Len);
/* USER CODE END PV */
```

*Function definition replacement*
```c
static int8_t CDC_Receive_HS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 11 */
    return (int8_t) USB_on_receive(Buf, Len);
  /* USER CODE END 11 */
}
```

This will move the callback behaviour definition to the ```driver_usb.c``` file.

## User button (driver_user_button.h/c)
*files* : `driver_usb.h` , `driver_usb.c`

Driver to handle user button on PA15 pin.

**Requirements:**
This driver needs the EXTI wrapper (driver_exti.c/h) lib.

**Pinout:**
- Configure pin PA15 as `GPIO_EXTI15`
- System Core/GPIO/
    * GPIO/Pin Name = PA15(JTDI):
        * GPIO mode = External Interrupt Mode with Rising/Falling edge trigger detection
    * NVIC:
        * EXTI line[15:10] interrupts : enable by checking

## FDCAN / CAN

Please refer to [ZDT stm32 driver](https://github.com/lgabp1/zdt_stm32_driver) and use the driver_CAN related files only.

Please also refer to that project for CAN setup.

## Adressable LED WS2812 (ws2812.c/h)
*files* : `ws2812.h` , `ws2812.c` (provided by the board's manufacturer [here](https://gitee.com/kit-miao/dm-mc02/tree/master/%E4%BE%8B%E7%A8%8B/CtrBoard-H7_WS2812))

Library that I did not write but I found useful. Driver to control the WS2812 LED.

**Pinout:**
- Connectivity/SPI6:
    * Transmit Only Master (Should enable PA5 and PA7)

**Clock config:**
The WS2812 LED is connected to SPI6, which is connected to the APB4 Peripheral clocks, which needs to be set to 120MHz.

**RQ: TEST IF NECESSARY**
Set under SPI6 Clock Mux, source as HSE. The baud rate of SPI6 should be 12Mbit/s
![SPI6_mux](https://github.com/user-attachments/assets/d36ca13a-fd74-44ea-b66a-53886b7eedf8)

# Driver usage

## EXTI wrapper usage

**Requirements** : None, but is required for a few drivers.

**Example program**
```c
/* Configure actions for button press */
void USER_GPIO_15_EXTI_CALLBACK()
{
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)) { // Rising edge
      light_on();
  } else { // Falling edge
      light_off();
  }
}
```

## Servo PWM usage

**Requirements** : Please be sure to copy the required files in the project folder and set up the pin and clocks accordingly.

**Example program**

```C
    /* Configure servomotor */
    servo_PWM servo1;
    servo1.htim = htim1;
    servo1.channel = TIM_CHANNEL_3;
    servo1.min_angle = -135; servo1.max_angle = 135;
    servo1.min_duty = 0.025; servo1.max_duty = 0.125;
    servo_PWM_start(servo1);

    /* Drive the servo motor */
    servo_PWM_set_angle(servo1, -100);
    HAL_Delay(1000);
    servo_PWM_set_angle(servo1, 100);
```

Below is an example use of simultaneous drive (assuming servo1, ..., servo4 already defined)
```c
  servo_PWM* servos[4] = {&servo1, &servo2, &servo3, &servo4};
  float angles_deg[4];
  float max_speeds_degps[4] = {50, 40, 30, 20};
  while (1) {
	  angles_deg[0] = -100.0f; angles_deg[1] = -80.0f; angles_deg[2] = -60.0f; angles_deg[3] = -40.0f;
	  servo_PWM_group_goto_angle(servos, angles_deg, max_speeds_degps, 4);
	  HAL_Delay(500);

	  angles_deg[0] = 100.0f; angles_deg[1] = 80.0f; angles_deg[2] = 60.0f; angles_deg[3] = 40.0f;
	  servo_PWM_group_goto_angle(servos, angles_deg, max_speeds_degps, 4);
	  HAL_Delay(500);
  }
```

## Buzzer usage

**Requirements** : Please be sure to copy the required files in the project folder and set up the pin and clocks accordingly.

**Example program**
```C
    /* Drive the buzzer */
    buzzer_enable();
    buzzer_set_midi(90);
    HAL_Delay(200);
    buzzer_disable();
```

## USB serial usage

**Requirements** : Please be sure to copy the required files in the project folder and set up the pin and clocks accordingly. Please also update the `./USB_DEVICE/APP/usbd_cdc_if.c` file as instructed.

**Example program**
```C
/* Retrieve data from uint8_t serial_buffer[USB_BUFFER_SIZE]; defined in driver_usb.c*/

/* Optional definition, define user callback */
int USB_user_callback() {
    char* buffer = "Hey I have received data!\n";
    USB_transmit((uint8_t*)buffer, strlen(buffer));
    return 0;
}

int main(void) {
    /* ... */

    /*...*/
    int counter = 0;
    while (1)
    {
#define BUF_SIZE 100
        char *buffer = (char *)malloc(BUF_SIZE);
        snprintf(buffer, BUF_SIZE, "Hello World from USB %d\n", counter);
        
        USB_transmit((uint8_t*)buffer, strlen(buffer));
        
        counter ++;
        free(buffer);
        HAL_Delay(1000);
    }
}
```

## User button usage

**Requirements** : Please be sure to copy the required files in the project folder and set up the pin accordingly.

**Example program**
```C
/* Optional definition, define user callback */
int button_pressed;
void user_button_release_callback() {
    button_pressed = 1 - button_pressed;
}

int main(void) {
    /* ... */

    /*...*/
    int counter = 0;
    while (1)
    {
      int a = user_button_read();
      if (a) {
        break;
      }
    }
}
```

## FDCAN / CAN usage

Please refer to [ZDT stm32 driver](https://github.com/lgabp1/zdt_stm32_driver) for usage information.

## Adressable LED WS2812 usage

**Requirements** : Please be sure to copy the required files in the project folder and set up the pin and clocks accordingly.

**Example program**
```C
    /* Drive the LED */
    WS2812_Ctrl(250,0,0);
    HAL_Delay(200);
    WS2812_Ctrl(0,250,0);
    HAL_Delay(200);
    WS2812_Ctrl(0,0,250);
```
