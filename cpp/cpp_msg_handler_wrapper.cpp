#include "cpp_msg_handler_wrapper.hpp"
#include "cpp_thead_safe_queue.hpp"
#include <thread>
#include <chrono>
#include <ranges>
#include <iostream>

using namespace std;
using namespace cpp_to_python_win32pipe;

const int sleep_time = 5; // Sleep time in ms

void work_thread(bool* processing_messages, cpp_to_python* msg_receiver, map<string, cpp_thead_safe_queue*>* listeners){
    while(processing_messages){
        if(!msg_receiver->is_connected()) continue; // Do not process messages if we are not connected
        std::vector<packet_message*>* msgs = msg_receiver->check_msgs();
        if(msgs == nullptr) continue; // No msgs to process
        for (int i = 0; i < msgs->size(); i++){
            packet_message* msg = msgs->at(i);
            string id = msg->data_id;
            if(listeners->find(id) != listeners->end()){
                cout << "Added msg to ID: " << id << endl;
                listeners->at(id)->push(msg);
            } else {
                cout << "Print and dropped a message: " << msg->data_id << endl;
                delete msg->data_payload;
                delete msg;
            }
        }
        delete msgs;
        this_thread::sleep_for(chrono::milliseconds(3));
    }
}

void cpp_msg_handler_wrapper::process_messages(){
    thread work_thead(work_thread, &processing_messages, msg_receiver, &listeners);
    work_thead.detach();
}

cpp_msg_handler_wrapper::cpp_msg_handler_wrapper() :
    processing_messages{true} ,
    internal_connection{true} ,
    msg_receiver{new cpp_to_python()}{
    process_messages();
}

cpp_msg_handler_wrapper::cpp_msg_handler_wrapper(cpp_to_python* msg_rec) : 
    processing_messages{true} ,
    internal_connection{false} ,
    msg_receiver{msg_rec}{
    process_messages();
}

cpp_msg_handler_wrapper::~cpp_msg_handler_wrapper(){
    processing_messages = false;
    if(internal_connection) delete msg_receiver;
}

void cpp_msg_handler_wrapper::add_listener(string id){
    listeners[id] = new cpp_thead_safe_queue();
}

std::vector<packet_message*>* cpp_msg_handler_wrapper::get_message_for_id(string id){
    cpp_thead_safe_queue* queue = listeners.at(id);
    int total = 0;
    if(queue->count() == 0) return nullptr;
    std::vector<packet_message*>* ret_val = new std::vector<packet_message*>();
    while(queue->count() > 0 && total <= MAX_CHECK_COUNT){
        ret_val->push_back(queue->pop());
        total++;
    }
    return ret_val;
}

void cpp_msg_handler_wrapper::retry_connection(){
    msg_receiver->retry_connection();
}

bool cpp_msg_handler_wrapper::is_connected(){
    return msg_receiver->is_connected();
}

void cpp_msg_handler_wrapper::send_msg(std::string id, int message_size, unsigned char* byte_message){
    msg_receiver->send_msg(id, message_size, byte_message);
}