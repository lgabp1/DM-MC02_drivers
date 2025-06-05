"""
Defines a class to handle serial communication
"""

import time
from serial import Serial, SerialException
from serial.tools.list_ports import comports
from serial.tools.list_ports_common import ListPortInfo
from threading import Lock

def get_serial_ports() -> list[ListPortInfo]:
    """Get a list of available serial ports.
    
    Return: list[ListPortInfo]: A list of available serial ports."""
    return [port.device for port in comports()]

class SerialHandler:
    """Class to handle serial communication"""
    locks: dict[str, Lock] = {} # Locks for all serial handles

    
    def __init__(self, port: str, baudrate: int, timeout: float = 1.0):
        """Class to handle serial communication
        
        Args:
            port (str): The serial port to connect to. Example: 'COM3' on Windows or '/dev/ttyUSB0' on Linux.
            baudrate (int): The baud rate for the serial communication. Example: 9600.
            timeout (float, optional): The timeout for the serial communication. Defaults to 1.0."""
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout
        SerialHandler.locks[port] = Lock()
        
        self.serial = Serial()
        self.serial.port, self.serial.baudrate, self.serial.timeout = port, baudrate, timeout
    
    def open(self) -> None:
        """Open the serial port"""
        self.serial.open()
    
    def is_open(self) -> bool:
        """Check if the serial port is open.
        
        Return: bool: True if the serial port is open, False otherwise."""
        return self.serial.is_open

    def close(self) -> None:
        """Close the serial port."""
        if self.is_open(): 
            self.serial.close()
        else:
            raise SerialException("Serial port is not open")

    def send(self, data: bytearray) -> None:
        """Send data to the serial port."""
        if self.is_open():
            with SerialHandler.locks[self.port]:
                self.serial.write(data)
        else:
            raise SerialException("Serial port is not open")

    def receive_line(self, encoding: str = 'ascii', remove_newline: bool = True) -> str:
        """Receive a line of data from the serial port.
        
        Args: 
           timeout (float, optional): The timeout in seconds for the serial communication. Defaults to 1.0.
           encoding (str, optional): The encoding for the received data. Defaults to 'ascii'."""
        if self.is_open():  # Check if the serial port is open
            with SerialHandler.locks[self.port]:
                line = self.serial.readline().decode(encoding)  # Read a line of data and decode it to a string
            if remove_newline:
                line = line.replace('\n', '').replace('\r', '')  # Remove newline characters from the received data
            return line
        else:
            raise SerialException("Serial port is not open")
