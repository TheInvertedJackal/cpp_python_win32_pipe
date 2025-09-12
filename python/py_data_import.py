# From: https://stackoverflow.com/questions/48542644/python-and-windows-named-pipes

import time
import sys
import win32pipe, win32file, pywintypes

def pipe_client():
    print("pipe client")
    quit = False

    while not quit:
        try:
            handle = win32file.CreateFile(
                r'\\.\pipe\cpp_to_python_pipe',
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                win32file.FILE_ATTRIBUTE_NORMAL,
                None
            )
            try:
                err, bytes_written = win32file.WriteFile(handle, b"Hello from python!")
                if err == 0:  # No error
                    print(f"Sent {bytes_written} bytes to the pipe.")
                    time.sleep(5)
                else:
                    print(f"Error writing to pipe: {err}")
                    time.sleep(5)
            except win32file.error as e:
                print(f"Error writing to pipe: {e}")
                time.sleep(5)
            
            # print ("Setting the Pipe handle state...")
            # res = win32pipe.SetNamedPipeHandleState(handle, win32pipe.PIPE_READMODE_MESSAGE, 10, None)
            # print("Set!")
            # if res == 0:
            #     print(f"SetNamedPipeHandleState return code: {res}")
            # while True:
            #     print("Message Preped!")
            #     win32file.WriteFile(handle ,"Hello from python!")
            #     print("Message Sent!")
            #     time.sleep(5)
        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again in a sec")
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe, bye bye")
                quit = True
