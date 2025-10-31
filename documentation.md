# On C++ to Python

## C++

Using the sending and receiving of messages in C++
1. include cpp pipe manager
2. Before you use any methods ensure that you're connected with the is_connected() method.
3. If not retry_connection()
4. Once connected you can receive messages with check_messages() and it'll return a list if there are any. They come in the form of a struct with the size of the payload, the payload and an id.
5. To send messages use the send_msg() method with an id, byte payload and size.

## Python

Very much like python
1. import PythonToCpp() from the py_data_packckager
2. Try for is_connected()
3. If not connected, retry_connection()
4. Once connected get_msgs() will return a list with a dictonary of messages, containing the id and byte string content.
5. Send messages with send_msg() and include an id (max of 16 chrs) and a byte string payload.

## The How

Win32 pipes allow for inter process communication, just like Linux pipes. However unlike Linux Pipes, Windows pipes use named files, and in addition carry the size of each message, making transport a bit easier.

The system is multi threaded, however all of the methods that you'll be using are thread save, so don't worry.

The order of launching applications may matter, I'll try to hammer out this bug, but we'll see in the future.

Most methods are commented and documented that way.