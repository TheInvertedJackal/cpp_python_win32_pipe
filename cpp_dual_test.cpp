#include "cpp/cpp_pipe_handler.hpp"
#include "cpp/cpp_msg_handler_wrapper.hpp"
#include <thread> // For std::this_thread::sleep_for
#include <chrono>
#include <iostream>

using namespace cpp_to_python_win32pipe;

int main(){
    //cpp_to_python to_from_py = cpp_to_python();
    cpp_msg_handler_wrapper sorter = cpp_msg_handler_wrapper();
    sorter.add_listener("reset");
    //sorter.add_listener("print_str");
    bool msg_sent = false;
    while(true){
        // Try to connect if you can't sleep
        if(!sorter.is_connected()){
            std::cout << "I'm not connected! Help!" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            sorter.retry_connection();
        } else {
            if(!msg_sent){
                std::string message = "Hello from the C++ manager!";
                std::string rst_msg = "Please reset!";
                sorter.send_msg("ofical_c++", message.length(), (unsigned char*)message.c_str());
                sorter.send_msg("reset", rst_msg.length(), (unsigned char*)rst_msg.c_str());
                msg_sent = true;
            }
            // Try to read messages
            //std::vector<packet_message*>* possible_msgs = to_from_py.check_msgs();
            std::vector<packet_message*>* possible_msgs = sorter.get_message_for_id("reset");
            if(possible_msgs){
                std::cerr << "Got msgs!" << std::endl;
                for (size_t i = 0; i < possible_msgs->size(); i++){
                    std::cerr << "Looped: " << i << std::endl;
                    packet_message* msg = possible_msgs->at(i);
                    std::cerr << std::string((const char*)msg->data_payload) << std::endl; 
                    // Cleanup!
                    delete[] msg->data_payload;
                    delete msg;
                }
                std::cerr << "Exited loop" << std::endl << std::flush;
                delete possible_msgs;
                std::cerr << "Cleaned up msgs!" << std::endl << std::flush;
            }
        }
    }
}