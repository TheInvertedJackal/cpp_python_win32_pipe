#include <iostream>
#include <cassert>
#include "byte_utils.hpp"

using namespace std;
using namespace byte_utils;

unsigned char data_one[] = {0x00, 0x00, 0x00, 0x0F};

int main(){
    cout << "Starting Tests" << endl;
    // Flipping Bits
    assert(reverse_bits((unsigned char) 0b11001010), (unsigned char) 0b01010011);
    assert(byte_to_int(data_one, 0));
    return 0;
}