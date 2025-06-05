import logging
from pathlib import Path

class FilePrintLogger(logging.Logger):
    """A logger allowing file and terminal logging."""
    LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"

    def __init__(self, file_level: int = logging.INFO, file_path: str = f"{Path(__file__).parent / 'logs.log'}", console_level: int = logging.INFO, name: str = "fileprintlogger"):
        """A logger allowing file and terminal logging.
        
        Args:
            file_level (int): The level of the file logger. Set to -1 to disable.
            console_level (int): The level of the console logger. Set to -1 to disable.
            """
        super().__init__(name, max(file_level, console_level))
        
        if file_level >= 0: # add file logger if file_level is not negative
            with open(file_path, "a") as f: # create the file if not yet exist
                f.write("")
            file_handler = logging.FileHandler(file_path, encoding="utf-8")
            file_handler.setLevel(file_level)
            file_handler.setFormatter(logging.Formatter(FilePrintLogger.LOG_FORMAT))
            self.addHandler(file_handler)
        
        if console_level >= 0: # add console logger if console_level is not negative
            console_handler = logging.StreamHandler()
            console_handler.setLevel(console_level)
            console_handler.setFormatter(logging.Formatter(FilePrintLogger.LOG_FORMAT))
            self.addHandler(console_handler)

    def setLevel(self, level: int) -> None:
        """Set the level of the logger and all handlers."""
        super().setLevel(level)
        for handler in self.handlers:
            handler.setLevel(level)
