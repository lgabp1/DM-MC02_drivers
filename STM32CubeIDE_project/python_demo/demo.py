from lib.lib_serial import SerialHandler, SerialException, get_serial_ports
import threading
import time
from logging import Logger
from typing import Optional
from lib.lib_logging import FilePrintLogger

from lib.lib_thread import ThreadedSerialHandler

class DMMC02HardwareWrapper:
    """Simple class to wrap everything low-level related"""
    def __init__(self, logger: Optional[Logger] = None):
        self.serial_handler  = ThreadedSerialHandler(logger=logger)
    
    def set_servo_angle(self, servo_index: int, angle: float) -> None:
        """Set the angle of a servo motor."""
        msg = f"1,{servo_index},{angle}"
        self.serial_handler.queue_message(msg)
    
    def set_led_color(self, r:int, g:int, b:int) -> None:
        """Set the color of the LED."""
        msg = f"0,{r},{g},{b}"
        self.serial_handler.queue_message(msg)
    
    def set_buzzer_midi(self, midi_id: int) -> None:
        """Set the buzzer to play a MIDI note."""
        msg = f"2,{midi_id}"
        self.serial_handler.queue_message(msg)


if __name__ == "__main__":
    logger = FilePrintLogger(file_level=-1, console_level=10) # only console

    wrap = DMMC02HardwareWrapper(logger)    
    wrap.set_led_color(0,0,0)
    
    for i in range(255):
        r = i%2*255
        g = (i//2)%2*255
        b = (i//4)%2*255
        wrap.set_led_color(r,g,b)
        time.sleep(0.1)

    while True:
        time.sleep(2)
        wrap.set_servo_angle(1,-100)
        time.sleep(0.25)
        wrap.set_servo_angle(4,100)
        time.sleep(0.25)
        wrap.set_servo_angle(2,-100)
        time.sleep(0.25)
        wrap.set_servo_angle(3,100)

        time.sleep(2)
        wrap.set_servo_angle(1,-50)
        time.sleep(0.25)
        wrap.set_servo_angle(4,50)
        time.sleep(0.25)
        wrap.set_servo_angle(2,-50)
        time.sleep(0.25)
        wrap.set_servo_angle(3,50)
        
        time.sleep(2)
        wrap.set_servo_angle(1, 0)
        time.sleep(0.25)
        wrap.set_servo_angle(4, 0)
        time.sleep(0.25)
        wrap.set_servo_angle(2,0)
        time.sleep(0.25)
        wrap.set_servo_angle(3,0)
        
        time.sleep(2)
        wrap.set_servo_angle(1,50)
        time.sleep(0.25)
        wrap.set_servo_angle(4,-50)
        time.sleep(0.25)
        wrap.set_servo_angle(2,50)
        time.sleep(0.25)
        wrap.set_servo_angle(3,-50)

        time.sleep(2)
        wrap.set_servo_angle(1, 100)
        time.sleep(0.25)
        wrap.set_servo_angle(4, -100)
        time.sleep(0.25)
        wrap.set_servo_angle(2,100)
        time.sleep(0.25)
        wrap.set_servo_angle(3,-100)