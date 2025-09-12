#include "cpp_data_transfer_manager.hpp"

using namespace std;
using namespace CppToPy_Win32;




// NO LOOP!
// Management Process
// Setup empty map
// Created shared memeory struct
// Spawn the Export Thread (Detached)
// Spawn the Improt Thread (Detached)
cpp_data_transfer_manager::cpp_data_transfer_manager(){
    callback_lookup_map = map<std::string, payload_function>();
    t_data = new thread_shared_data;
    // Set up shared data
    t_data->other_program_connected = false;
    t_data->other_program_ready = false;
    t_data->payload_1_up=false;
    t_data->payload_2_up=false;
}

// Deconstructor
// N/A (Can't really terminate threads etc)
cpp_data_transfer_manager::~cpp_data_transfer_manager(){
    delete t_data;
}

// set_payload_values()
// Check and see if 1 is filled
// Check and see if 2 is filled
// If both are filled drop the packet and print an error
void cpp_data_transfer_manager::set_payload_values(std::string data_id, uint32_t data_size, unsigned char* data_payload){

}

// signal_ready()
// singal the other program that you're set up
void cpp_data_transfer_manager::singal_ready(){
    set_payload_values(ready_str, 0, nullptr);
}

bool cpp_data_transfer_manager::is_connected(){
    return t_data->other_program_ready;
}

// post_payload()
// place paylod info in shared data
// flip boolean
void cpp_data_transfer_manager::post_payload(std::string data_id, uint32_t data_size, unsigned char* data_payload){
    if(t_data->other_program_ready) throw new exception("Error: You cannot sent messages untill the other program is ready. Use is_connected() to check.");

}

// add_callback()
// Add callback to map
void cpp_data_transfer_manager::add_callback(std::string id, payload_function fn){
    callback_lookup_map[id] = fn;
}

// get_callback()
// Return a callback, if not in the map, return null pointer
payload_function cpp_data_transfer_manager::get_callback(std::string id){
    if (callback_lookup_map.find(id) == callback_lookup_map.end()) return nullptr;
    return callback_lookup_map[id];
}