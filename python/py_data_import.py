# From: https://stackoverflow.com/questions/48542644/python-and-windows-named-pipes

import time
import sys
import win32pipe, win32file, pywintypes
from python.py_msg_box import MessageBox
from python.py_shared_encode_data import buffer_size
from python.py_data_decoder import data_decoder
from python.python_log import print_log


# This created and managed asyncronisly!
class data_import_manager:
    message_box: MessageBox = 0
    pipe_h = 0

    def __init__(self, shared_data_class: MessageBox):
        self.message_box = shared_data_class
        print_log("Setting up Python Server")
        self.pipe_h = win32pipe.CreateNamedPipe(
            r'\\.\pipe\cpp_to_python_pipe',
            win32pipe.PIPE_ACCESS_DUPLEX,
            win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT,
            1, buffer_size * 64, buffer_size * 64,
            0,
            None)
        print_log("Waiting for C++ Client")
        win32pipe.ConnectNamedPipe(self.pipe_h, None)
        print_log("Connected to C++ Client")
        try:
            self.read_data()
        finally:
            win32file.CloseHandle(self.pipe_h)  

    def read_data(self):
        while True:
            try:
                print_log("Preped to Recive Msg")
                resp = win32file.ReadFile(self.pipe_h, 64 * buffer_size)
                # res = win32pipe.SetNamedPipeHandleState(self.pipe_h, win32pipe.PIPE_READMODE_MESSAGE, None, None)
                # if res == 0:
                #     print(f"SetNamedPipeHandleState return code: {res}")
                print_log("Recived Message from C++ Client")
                if resp[0] == 0:
                    self.message_box.place_message(data_decoder(resp[1]))
                else:
                    print_log("Data not read correctly!")
            except pywintypes.error as e:
                if e.winerror == 109:
                    print_log("Client Disconnected.")
                    break
                else:
                    raise

def threaded_data_import(shared_data_class: MessageBox):
    importer = data_import_manager(shared_data_class)