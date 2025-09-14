#include "cpp/cpp_pipe_methods.hpp"
#include <iostream>

int main(){
    std::cout << "Started client!" << std::endl;
    cpp_to_python_win32pipe::threaded_data_import(nullptr);
    std::cout << "Ended client!" << std::endl;
    return 0;
}