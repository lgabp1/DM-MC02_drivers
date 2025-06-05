import threading
from .lib_serial import SerialHandler, get_serial_ports, SerialException
import time
from typing import Callable, Any, Optional, Literal
from logging import Logger

class ThreadedSerialHandler:
    """Wrapper of SerialHandler with auto-reconnecting behaviour and safety measures."""
    def __init__(self, baudrate: int=115200, timeout: float=1.0, wait_time: float = 0.1, logger: Optional[Logger] = None):
        self._lock = threading.Lock()
        self._running = False
        self._serial_handler: SerialHandler = None
        self.logger = logger

        self._baudrate, self._timeout, self._wait_time = baudrate, timeout, wait_time
        self._message_queue: list[str] = []

        self.start_threaded()
        
    def queue_message(self, msg: str) -> None:
        """Queue a message to be sent to the serial port."""
        if self._serial_handler and self._serial_handler.is_open():
            with self._lock:
                self._message_queue.append(msg)

    def _loop(self):
        self._running = True
        port = None
        while self._running:
            try: 
                port = get_serial_ports()[0]
            except Exception:
                self._log("No serial port found.")
                time.sleep(self._wait_time)
                continue
            try: # Try connection
                if self._serial_handler is None:
                    self._log("SerialHandler is None: attempting to create a new instance...")
                    self._serial_handler = SerialHandler(port, self._baudrate, self._timeout)
                if self._serial_handler.is_open() is False:
                    self._log("Serial connection is not connected: attempting to connect...")
                    self._serial_handler.open()
                    self._message_queue.clear() # Avoid stacked queued messages
                    self._log(f"Serial connection established to port {port}.")
            except SerialException as e:
                self._log(f"Serial connection failed: {e}")
            
            try: # Try sending message
                if self._serial_handler.is_open():
                    if len(self._message_queue) > 0:
                        try: # try sending message
                            with self._lock:
                                msg = self._message_queue.pop(0)
                            self._serial_handler.send(msg.encode("ascii"))
                            self._log(f"Message sent to {port}: {msg}")
                        except SerialException as e:
                            self._log(f"Failed to send message: {e}")
                            self._serial_handler = None
                    else: # No message to send
                        pass
            except SerialException as e:
                self._log(f"Serial connection failed: {e}")
                self._serial_handler = None

            time.sleep(self._wait_time)

    def start_threaded(self):
        if self._running is False:
            with self._lock:
                self._running = True
            self._threader(self._loop)

    def _threader(self, target: Callable, **args: Any) -> None:  # create a thread
        thr = threading.Thread(target=self.__safe_thread, kwargs={"target": target, **args})
        thr.daemon = True
        thr.start()

    def __safe_thread(self, target: Callable, **args: Any) -> None:  # thread embedding
        try:
            target(**args)
        except KeyboardInterrupt:
            try:
                with self._lock:
                    self._running = False
            except Exception:
                pass
            pass
        except Exception as e:
            raise e from e
        finally:
            if self._serial_handler is not None:
                if self._serial_handler.is_open():
                    self._serial_handler.close()

    def _log(self, msg:str, log_level: Literal[10, 20, 30, 40, 50] = 10): # 10 = DEBUG, 20 = INFO, 30 = WARNING, 40 = ERROR, 50 = CRITICAL
        """Log given message to self.logger if is not None"""
        if self.logger:
            with self._lock:
                self.logger.log(log_level, msg)