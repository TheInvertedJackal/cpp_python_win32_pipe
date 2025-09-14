from python.py_shared_encode_data import prefix_magic_numbers, ending_magic_numbers, msg_id_size
from python.python_log import print_log

# Input: byte stirng and size

# Output: dict with an Id, and Byte String

def data_decoder(payload):
    min_size = len(prefix_magic_numbers) + len(ending_magic_numbers) + msg_id_size
    if len(payload) < min_size:
        print_log(f"Cannot process strings less than {min_size}")
        return 0
    
    passed_bytes = 0

    for i in range(len(prefix_magic_numbers)):
        if payload[i] != prefix_magic_numbers[i]:
            print_log("Message start did not line up with the reqired numbers!")
            return 0
    passed_bytes += len(prefix_magic_numbers)

    cut_value = msg_id_size
    for i in range(msg_id_size):
        if payload[passed_bytes + i] == 0:
            cut_value = i
            break
    passed_bytes += msg_id_size
    msg_id = payload[len(prefix_magic_numbers):len(prefix_magic_numbers) + cut_value].decode("utf-8")

    data_size = len(payload) - min_size
    data = payload[len(prefix_magic_numbers) + msg_id_size:-1 * len(ending_magic_numbers)]
    passed_bytes += data_size

    for i in range(len(ending_magic_numbers)):
        if payload[passed_bytes + i] != ending_magic_numbers[i]:
            print_log("Message ending did not line up with the reqired numbers!")
            return 0

    retval = {}
    retval["id"] = msg_id
    retval["data"] = data
    return retval