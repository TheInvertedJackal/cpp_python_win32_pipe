#ifndef TRANSFER_DATA_STRUCT_TO_PY
#define TRANSFER_DATA_STRUCT_TO_PY

#include <string>

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

#endif