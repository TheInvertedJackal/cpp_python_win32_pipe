#include "byte_utils.hpp"
#include <cstring>

using namespace byte_utils;

/*
    Determines if a computer is Big Endian, or Little Endian
*/
bool is_big_endian(){
    unsigned int test_value = 1; // A value where the least significant byte is 1
    char* byte_ptr = reinterpret_cast<char*>(&test_value);
    return *byte_ptr != 1;
}

/*
    Reverses the bits of a given byte
    Ex: 00100011 -> 11000100
    From: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
    value: The byte to reverse
*/
unsigned char reverse_bits(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

/*
    Reverses the bits of a given byte
    Ex: 00100011 -> 11000100
    From: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
    value: The byte to reverse
*/
template <typename T>
unsigned char* get_big_endian_byte_str(T value, size_t len){
    unsigned char* buffer = new unsigned char[len];
    memcpy(buffer, &value, len);
    if(!is_big_endian()){
        unsigned char* rev_buffer = new unsigned char[len];
        for (size_t i = 0; i < len; i++){
            rev_buffer[i] = reverse_bits(buffer[len - i - 1]);
        }
        delete[] buffer;
        buffer = rev_buffer;
    }
    return buffer;
}

/*
     Converts from a byte string to a value of type T
*/
template <typename T>
T byte_str_to_T(unsigned char* byte_array, int starting_index, size_t int_size){

}

unsigned char* byte_utils::int_to_byte(int value){
    size_t len = sizeof(int);
    return get_big_endian_byte_str<int>(value, len);
}

int byte_utils::byte_to_int(unsigned char* byte_array, int starting_index, size_t int_size){
    if(int_size > sizeof(int)) throw "Int Size cannot be greater than sizeof(int)!";

}

int byte_utils::byte_to_int(unsigned char* byte_array, int starting_index){
    return byte_utils::byte_to_int(byte_array, starting_index, sizeof(int));
}

unsigned char* byte_utils::double_to_byte(double value){
    size_t len = sizeof(double);
    return get_big_endian_byte_str<double>(value, len);
}

double byte_utils::byte_to_double(unsigned char* byte_array, int starting_index, size_t double_size){
    if(double_size > sizeof(double)) throw "Double Size cannot be greater than sizeof(double)!";
    if(is_big_endian()){

    }
}

double byte_utils::byte_to_double(unsigned char* byte_array, int starting_index){
    return byte_utils::byte_to_double(byte_array, starting_index, (size_t) sizeof(double));
}