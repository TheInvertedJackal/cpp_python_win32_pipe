from python.py_shared_encode_data import prefix_magic_numbers, ending_magic_numbers, msg_id_size
from python.python_log import print_log

# Input: Method args, Byte String (Payload) and An ID (Under 16 chrs but must be 16, if not replace with chrs)

# Output: Byte String

# Encodes data. Message ID and payload should be be byte strings.
def data_encoder(message_id, payload):
    retval = b""
    # Add starting Numbers
    retval += prefix_magic_numbers

    #Add message id
    if len(message_id) > msg_id_size:
        print_log("Message ID must be less than 16, otherwise it'll get cut off!")
        retval += message_id[:16]
    else:
        for _ in range(msg_id_size - len(message_id)):
            message_id += b"\x00"
        retval += message_id

    #Add Payload
    retval += payload

    # Add ending numbers
    retval += ending_magic_numbers
    return retval