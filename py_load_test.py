from python.py_data_packckager import PythonToCpp
import time

msg_count = 0
sent_msg_count = 0
manager = PythonToCpp()

print_msg_time = 0
last_time = 0

while True:
    current_time_ns = time.time_ns()
    if not manager.is_connected():
        manager.retry_connection()
    else:
        if current_time_ns - last_time >= 5000000 and sent_msg_count < 1000:
            sent_msg_count += 1
            last_time = current_time_ns
            manager.send_msg("print_str", b"PRINT THIS PRINT THIS NOW YOU FOOL!!!! AHALHJSDALKJDFLKJADESLQLKWEJHOLSKDFJHVLKH!~!!")
            sent_msg = True
    msgs = manager.get_msgs()
    if msgs != 0:
        for msg in msgs:
            msg_count += 1
    if current_time_ns - print_msg_time >= 1000000000:
        print_msg_time = current_time_ns
        print(f"Received {msg_count} messages.")