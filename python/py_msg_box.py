import threading
from python.python_log import print_log

reading_buffer_size = 128
max_retrys = 32

class MessageBox:
    avalible_msgs = 0
    next_place_buffer_index = reading_buffer_size - 1
    locks = []
    messages = []
    has_data_to_read = []

    def __init__(self):
        for _ in range(reading_buffer_size):
            self.locks.append(threading.Lock())
            self.messages.append({})
            self.has_data_to_read.append(False)

    def place_message(self, decoded_msg):
        message_placed = False
        trys = 0
        while not message_placed:
            if trys > max_retrys:
                print_log("Warning! Message has been dropped due to lack of buffer space!")
                break
            if self.locks[self.next_place_buffer_index].acquire(blocking=False):
                try:
                    if not self.has_data_to_read[self.next_place_buffer_index]:
                        self.has_data_to_read[self.next_place_buffer_index] = True
                        self.messages[self.next_place_buffer_index] = decoded_msg
                        message_placed = True
                        self.avalible_msgs += 1
                finally:
                    self.locks[self.next_place_buffer_index].release()
            trys += 1
            self.next_place_buffer_index -= 1
            if self.next_place_buffer_index < 0:
                self.next_place_buffer_index = reading_buffer_size - 1

    def get_messages(self):
        if self.avalible_msgs == 0:
            return 0
        retval = []
        for i in range(reading_buffer_size):
            if self.locks[i].acquire(blocking=False):
                try:
                    if self.has_data_to_read[i]:
                        self.has_data_to_read[i] = False
                        retval.append(self.messages[i])
                        self.avalible_msgs -= 1
                finally:
                    self.locks[i].release()
        return retval