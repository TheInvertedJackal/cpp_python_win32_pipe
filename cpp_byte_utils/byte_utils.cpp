#include "byte_utils.hpp"

using namespace byte_utils;

unsigned char* int_to_byte(int value){

}

int byte_to_int(unsigned char* byte_array, int starting_index, int int_size){

}

int byte_to_int(unsigned char* byte_array, int starting_index){

}

unsigned char* double_to_byte(double value){

}

double byte_to_double(unsigned char* byte_array, int starting_index){
    return byte_to_double(byte_array, starting_index, static_cast<int>(sizeof(double)));
}

double byte_to_double(unsigned char* byte_array, int starting_index, int double_size){
    if(double_size > sizeof(double)) throw "Double Size cannot be greater than sizeof(double)!";

}

