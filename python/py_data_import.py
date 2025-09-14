# From: https://stackoverflow.com/questions/48542644/python-and-windows-named-pipes

import time
import sys
import win32pipe, win32file, pywintypes


def pipe_server():
    print("pipe server")
    count = 0
    pipe_h = win32pipe.CreateNamedPipe(
        r'\\.\pipe\python_to_cpp_pipe',
        win32pipe.PIPE_ACCESS_DUPLEX,
        win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT,
        1, 65536, 65536,
        0,
        None)
    try:
        print("waiting for client")
        win32pipe.ConnectNamedPipe(pipe_h, None)
        print("got client")

        res = win32pipe.SetNamedPipeHandleState(pipe_h, win32pipe.PIPE_READMODE_MESSAGE, None, None)
        if res == 0:
            print(f"SetNamedPipeHandleState return code: {res}")
        resp = win32file.ReadFile(pipe_h, 64*1024)
        print(f"message: {resp}")  
    finally:
        win32file.CloseHandle(pipe_h)