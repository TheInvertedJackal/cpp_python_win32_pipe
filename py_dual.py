from python.py_data_packckager import PythonToCpp
import time

sent_msg = False
manager = PythonToCpp()


while True:
    if not manager.is_connected():
        manager.retry_connection()
    else:
        if not sent_msg:
            manager.send_msg("print_str", b"PRINT THIS PRINT THIS NOW YOU FOOL!!!! AHALHJSDALKJDFLKJADESLQLKWEJHOLSKDFJHVLKH!~!!")
            sent_msg = True
        msgs = manager.get_msgs()
        if msgs != 0:
            for msg in msgs:
                print(msg)
    time.sleep(1)