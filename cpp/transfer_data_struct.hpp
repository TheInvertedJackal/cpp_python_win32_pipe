#ifndef TRANSFER_DATA_STRUCT_TO_PY
#define TRANSFER_DATA_STRUCT_TO_PY

#include <string>
#include <mutex>

struct packet_message
{
    std::string data_id;
    int data_size;
    unsigned char* data_payload;
};

struct byte_string
{
    int size;
    unsigned char* data_str;
};

const int shared_msg_buffer_size = 128;

struct shared_data{
    int avalible_messages;
    std::mutex locks[shared_msg_buffer_size];
    packet_message* messages[shared_msg_buffer_size];
    bool data_to_read[shared_msg_buffer_size];
};

#endif