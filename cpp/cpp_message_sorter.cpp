#include "cpp_message_sorter.hpp"
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
        cout << "Got MSG" << endl;
        for(auto msg = msgs->begin(); msg != msgs->end(); ++msg){
            string id = (*msg)->data_id;
            if(listeners->find(id) != listeners->end()){
                cout << "Sorted msg into " << id << endl << flush;
                listeners->at(id)->push((*msg));
            }
        }
        delete msgs;
        this_thread::sleep_for(chrono::milliseconds(5));
    }
}

void cpp_msg_sorter::process_messages(){
    thread work_thead(work_thread, &processing_messages, msg_receiver, &listeners);
    work_thead.detach();
}

cpp_msg_sorter::cpp_msg_sorter(cpp_to_python* msg_rec) : 
    processing_messages{true} ,
    msg_receiver{msg_rec}{
    process_messages();
}

cpp_msg_sorter::~cpp_msg_sorter(){
    processing_messages = false;
}

void cpp_msg_sorter::add_listener(string id){
    listeners[id] = new cpp_thead_safe_queue();
}

std::vector<packet_message*>* cpp_msg_sorter::get_message_for_id(string id){
    cpp_thead_safe_queue* queue = listeners.at(id);
    int total = 0;
    if(queue->count() == 0) return nullptr;
    std::vector<packet_message*>* ret_val = new std::vector<packet_message*>();
    while(queue->count() > 0 || total <= MAX_CHECK_COUNT){
        ret_val->push_back(queue->pop());
        total++;
    }
    return ret_val;
}