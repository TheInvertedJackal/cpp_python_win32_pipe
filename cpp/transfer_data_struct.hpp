#ifndef TRANSFER_DATA_STRUCT_TO_PY
#define TRANSFER_DATA_STRUCT_TO_PY

#include <string>

struct transfer_data_struct
{
    std::string data_id;
    uint32_t data_size;
    uint8_t* data_payload;
};

#endif