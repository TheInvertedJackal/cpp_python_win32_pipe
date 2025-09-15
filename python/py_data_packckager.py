import threading
from python.python_log import print_log
from python.py_msg_box import MessageBox
from python.py_data_export import data_export_manager
from python.py_data_import import threaded_data_import

message_box: MessageBox = MessageBox()

class PythonToCpp:
    global message_box
    output: data_export_manager = 0

    def __init__(self):
    # Create Thread and Input Pipe
        thread = threading.Thread(target=threaded_data_import, args=(message_box,))
        thread.start()

    # Create output pipe
        self.output = data_export_manager()

    # Gets a list of messages from the reader, returns 0 if there are none
    def get_msgs(self):
        return message_box.get_messages()

    # Sends a messages to C++, must be connected to the pipe!
    def send_msg(self, id, byte_str_msg):
        if not self.output.is_connected():
            print_log("Pipe not connected, message not sent!")
        self.output.export_data(id, byte_str_msg)
    
    # True if pipe is connected, false if not 
    def is_connected(self):
        return self.output.is_connected()
    
    # Retry connecting the pipe to the server
    def retry_connection(self):
        self.output.retry_connection()