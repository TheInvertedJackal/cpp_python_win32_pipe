#ifndef CPP_TO_PY_BYTE_UTILS
#define CPP_TO_PY_BYTE_UTILS

#include <cstddef>

namespace byte_utils{
    
    /*
        Converts an integer to a big endian byte array
        Size of returned array is determined by sizeof(int)
        - value: The value to convert to a byte array
    */
    unsigned char* int_to_byte(int value);

    /*
        Converts a portion of a byte array to an integer.
        Assumes that it's formatted as a big endian value.
        - byte_array: The Byte Array to Read from
        - starting_index: The starting index to start reading from
        - int_size: # of bytes to read from the array, cannot be greater than sizeof(int)
    */
    int byte_to_int(unsigned char* byte_array, int starting_index, size_t int_size);

    /*
        Converts a portion of a byte array to an integer.
        Assumes that it's formatted as a big endian value.
        Uses sizeof(int) to determine how far to go
        - byte_array: The Byte Array to Read from
        - starting_index: The starting index to start reading from
    */
    int byte_to_int(unsigned char* byte_array, int starting_index);

    /*
        Converts a double to a big endian byte array
        Size of returned array is determined by sizeof(double)
        - value: The value to convert to a byte array
    */
    unsigned char* double_to_byte(double value);

    /*
        Converts a portion of a byte array to a double.
        Assumes that it's formatted as a big endian value.
        - byte_array: The Byte Array to Read from
        - starting_index: The starting index to start reading from
        - double_size: # of bytes to read from the array, cannot be greater than sizeof(double)
    */
    double byte_to_double(unsigned char* byte_array, int starting_index, size_t double_size);

    /*
        Converts a portion of a byte array to a double.
        Assumes that it's formatted as a big endian value.
        Uses sizeof(double) to determine how far to go
        - byte_array: The Byte Array to Read from
        - starting_index: The starting index to start reading from
    */
    double byte_to_double(unsigned char* byte_array, int starting_index);
}

#endif