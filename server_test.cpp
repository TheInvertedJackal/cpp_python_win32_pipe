// Dummy code, made to demonstrate the programs pourpose
#include <thread>
#include <chrono>

#include "cpp/cpp_pipe_methods.hpp"

using namespace cpp_to_python_win32pipe;

main(){
    data_export_manager exporter = data_export_manager();
    std::string message = "Hello!, my name is David. I'm on C++, but I'd love to talk to you python! Please return my call, I miss you python, I miss you dearly.";
    while(!exporter.is_connected()){
        cpp_to_python_win32pipe::print_log_message("No connection to python client, retrying in 1 second...");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        exporter.retry_connection();
    }
    exporter.export_data("hello_test", message.length(), (unsigned char*) message.c_str());
    return 0;
}