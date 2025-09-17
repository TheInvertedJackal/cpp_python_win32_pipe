#include "cpp/cpp_pipe_handler.hpp"
#include <thread> // For std::this_thread::sleep_for
#include <chrono>
#include <iostream>

using namespace cpp_to_python_win32pipe;

long long get_ns(){
    auto now = std::chrono::high_resolution_clock::now();

    // Convert the time point to a duration since the epoch
    auto nanoseconds_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(
        now.time_since_epoch()
    );

    // Get the count of nanoseconds
    return nanoseconds_since_epoch.count();

}


int main(){
    cpp_to_python to_from_py = cpp_to_python();
    int sent_msg_count = 0;
    int msg_count = 0;

    long long last_time = 0;
    long long print_msg_time = 0;

    while(true){
        long long current_time = get_ns();
        // Try to connect if you can't sleep
        if(!to_from_py.is_connected()){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            to_from_py.retry_connection();
        } else {
            if(current_time - last_time >= 5000000 && sent_msg_count < 1000){
                sent_msg_count++;
                last_time = current_time;
                std::string message = "Hello from the C++ manager! It's so cool it's C++ you can do anything in C++ so much cool things so much yes yes!";
                to_from_py.send_msg("offical_c++", message.length(), (unsigned char*)message.c_str());
            }
        }
        // Try to read messages
        std::vector<packet_message*>* possible_msgs = to_from_py.check_msgs();
        if(possible_msgs){
            msg_count += possible_msgs->size();
            for (size_t i = 0; i < possible_msgs->size(); i++){
                // Cleanup!
                delete[] possible_msgs->at(i)->data_payload;
                delete possible_msgs->at(i);
            }
            delete possible_msgs;
        }
        if(current_time - print_msg_time >= 1000000000){
            print_msg_time = current_time;
            std::cout << "We have " << msg_count << " messages!" << std::endl;
        }
    }
}