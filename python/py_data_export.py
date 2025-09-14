# From: https://stackoverflow.com/questions/48542644/python-and-windows-named-pipes
# FIRST STEP DONE! Connection established!

import win32pipe, win32file, pywintypes
from python.py_data_encoder import data_encoder
from python.python_log import print_log

class data_export_manager:
    h_pipe = 0
    connected = False

    def __init__(self):
        self.retry_connection()

    def retry_connection(self):
        try:
            self.h_pipe = win32file.CreateFile(
                r'\\.\pipe\cpp_to_python_pipe',
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                win32file.FILE_ATTRIBUTE_NORMAL,
                None
            )
            self.connected = True
        except pywintypes.error as e:
            if e.args[0] == 2:
                self.connected = False
            elif e.args[0] == 109:
                print_log("Major Error: Pipe is broken.")
                exit(0)

    def is_connected(self):
        return self.connected
    
    def export_data(self, id, byte_str):
        if isinstance(id, str):
            id = id.encode("utf-8")
        encoded_data = data_encoder(id, byte_str)
        try:
            err, bytes_written = win32file.WriteFile(self.h_pipe, encoded_data)
            if err == 0:  # No error
                print(f"Sent {bytes_written} bytes to the pipe.")
            else:
                print_log(f"Error writing to pipe: {err}")
                return 0
        except win32file.error as e:
            print_log(f"Error writing to pipe: {e}")
