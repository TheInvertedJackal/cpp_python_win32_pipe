#include "cpp_pipe_handler.hpp"
#include <thread>

using namespace cpp_to_python_win32pipe;

cpp_to_python::cpp_to_python(){
    // Set up shared memeory
    message_box = new shared_data;
    message_box->avalible_messages = 0;
    for (int i = 0; i < shared_msg_buffer_size; i++){
        message_box->messages[i] = nullptr;
        message_box->data_to_read[i] = false;
    }

    // Create out Pipe
    exporter = data_export_manager();
    exporter.retry_connection();

    // Create in pipe (Thread)
    std::thread importer_t(threaded_data_import, message_box);
    importer_t.detach();
}

std::vector<packet_message*>* cpp_to_python::pull_msgs(){
    std::vector<packet_message*>* ret_list = new std::vector<packet_message*>();
    for(int i = 0; i < shared_msg_buffer_size; i++){
        if(message_box->locks[i].try_lock()){
            if(message_box->data_to_read[i]){
                ret_list->push_back(message_box->messages[i]);
                message_box->data_to_read[i] = false;
                message_box->avalible_messages -= 1;
            }
            message_box->locks[i].unlock();
        }
    }
    return ret_list;
}

std::vector<packet_message*>* cpp_to_python::check_msgs(){
    // if(!exporter.is_connected()) print_error_and_crash("You need to be connected to python to read messages.");
    if(message_box->avalible_messages == 0) return nullptr;
    return pull_msgs();
}

bool cpp_to_python::is_connected(){
    return exporter.is_connected();
}

void cpp_to_python::retry_connection(){
    print_log_message("Retrying Connection");
    exporter.retry_connection();
}

void cpp_to_python::send_msg(std::string id, int message_size, unsigned char* byte_message){
    if(!exporter.is_connected()) print_error_and_crash("Exporter is not connected to python, cannot send message.");
    exporter.export_data(id, message_size, byte_message);
}