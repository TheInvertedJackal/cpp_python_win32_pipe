# Dummy code, made to demonstrate the point of the program
import time

from python.py_data_export import data_export_manager
from python.python_log import print_log

exporter = data_export_manager()
while not exporter.is_connected():
    print_log("Python could not connect to C++ Server, retrying in 1 second...")
    time.sleep(1)
    exporter.retry_connection()
exporter.export_data("print_str", b"Hello my sweet C++, I love you so much!!! You make my day bringer! You're so much better and faster than I am! I wish I was like you!\x00")
while True:
    one = 1