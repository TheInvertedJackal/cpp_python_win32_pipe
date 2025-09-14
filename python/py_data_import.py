# From: https://stackoverflow.com/questions/48542644/python-and-windows-named-pipes

import time
import sys
import win32pipe, win32file, pywintypes
from python.py_shared_encode_data import buffer_size
from python.py_data_decoder import data_decoder
from python.python_log import print_log

# This created and managed asyncronisly!
class data_import_manager:
    pipe_h = 0
    
    # TODO ADD Shared data!!!
    def __init__(self):
        print_log("Setting up Python Server")
        self.pipe_h = win32pipe.CreateNamedPipe(
            r'\\.\pipe\python_to_cpp_pipe',
            win32pipe.PIPE_ACCESS_DUPLEX,
            win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT,
            1, buffer_size * 64, buffer_size * 64,
            0,
            None)
        print_log("Server Created")
        print_log("Waiting for C++ Client")
        try:
            win32pipe.ConnectNamedPipe(self.pipe_h, None)
            print_log("Connected to C++ Client")
            self.read_data()
        finally:
            win32file.CloseHandle(self.pipe_h)  

    def read_data(self):
        while True:
            res = win32pipe.SetNamedPipeHandleState(self.pipe_h, win32pipe.PIPE_READMODE_MESSAGE, None, None)
            if res == 0:
                print(f"SetNamedPipeHandleState return code: {res}")
            resp = win32file.ReadFile(self.pipe_h, 64 * buffer_size)
            print(data_decoder(resp[1]))

# TODO Add shared data!!!!

def threaded_data_import():
    importer = data_import_manager()