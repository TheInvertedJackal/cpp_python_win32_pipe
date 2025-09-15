#include <iostream>

#include "cpp_pipe_methods.hpp"

// Base from: https://stackoverflow.com/questions/26561604/create-windows-named-pipe-in-c

void cpp_to_python_win32pipe::print_log_message(std::string message){
    std::cout << "[C++ Side] - " << message << std::endl << std::flush;
}

void cpp_to_python_win32pipe::print_error_and_crash(std::string error_message){
    std::cerr << "[C++ Side] - " << error_message << std::endl;
    throw error_message;
}

// Data Importing

cpp_to_python_win32pipe::data_import_manager::data_import_manager(shared_data* message_box){
    this->message_box = message_box;
    cpp_to_python_win32pipe::print_log_message("Creating Import Pipe");
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\python_to_cpp_pipe"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                            1,
                            BUFFER_SIZE * 16,
                            BUFFER_SIZE * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    cpp_to_python_win32pipe::print_log_message("Import Pipe Created and Checking for Client");
    check_for_client();
}

void cpp_to_python_win32pipe::data_import_manager::check_for_client(){
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            cpp_to_python_win32pipe::print_log_message("Pipe Connetced to Client");
            read_messages();
        }
        DisconnectNamedPipe(hPipe);
    }
}

void cpp_to_python_win32pipe::data_import_manager::read_messages(){
    while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE){
        cpp_to_python_win32pipe::print_log_message("Message from Client recived.");
        // Read and decode message
        packet_message* message = decode_message((int) dwRead, buffer);

        // Do something with said message!
        place_message(message);
    }
}

void cpp_to_python_win32pipe::data_import_manager::place_message(packet_message* message){
    bool message_placed = false;
    int trys = 0;
    while (!message_placed){
        if(trys > max_msg_placement_tries){
            print_log_message("Warning, message dropped!");
            delete[] message->data_payload;
            delete message;
            break;
        }
        // Check to see if box is being read
        if(message_box->locks[next_message_index].try_lock()){
            // If we have a free box
            if(!message_box->data_to_read[next_message_index]){
                message_box->messages[next_message_index] = message;
                message_box->data_to_read[next_message_index] = true;
                message_box->avalible_messages += 1;
                message_placed = true;
            }
            message_box->locks[next_message_index].unlock();
        }
        // Move onto the next one
        next_message_index -= 1;
        trys += 1;
        // Possible reset
        if (next_message_index < 0) next_message_index = shared_msg_buffer_size - 1;
    }
}

packet_message* cpp_to_python_win32pipe::data_import_manager::decode_message(int size, unsigned char* data){
    int meta_data_size = magic_numbers_count * 2 + message_id_size; // First 4 + ID 16 + Last 4
    if (size < meta_data_size) cpp_to_python_win32pipe::print_error_and_crash("Cannot decode a message that is less than 24 bytes!");
    int passed_bytes = 0;

    // Check if the first 4 bytes are the magic bytes
    for(int i = 0; i < cpp_to_python_win32pipe::magic_numbers_count; i++){
        if(buffer[i] != prefix_magic_numbers[i]) cpp_to_python_win32pipe::print_error_and_crash("Prefix Numbers of Message were not correct");
    }
    passed_bytes += cpp_to_python_win32pipe::magic_numbers_count;

    // Check the next 16 bytes for ID
    char message_id[message_id_size + 1];
    for(int i = 0; i < message_id_size; i++){
        message_id[i] = (char) buffer[passed_bytes + i];
    }
    // Add Trailing 0!
    message_id[message_id_size] = '\0';
    passed_bytes += cpp_to_python_win32pipe::message_id_size;

    // Get payload
    int payload_size = size - meta_data_size;
    unsigned char* payload = new unsigned char[payload_size];
    for(int i = 0; i < payload_size; i++){
        payload[i] = buffer[passed_bytes + i];
    }
    passed_bytes += payload_size;
    
    // Check final 4 bytes
    for(int i = 0; i < cpp_to_python_win32pipe::magic_numbers_count; i++){
        if(buffer[passed_bytes + i] != ending_magic_numbers[i]) cpp_to_python_win32pipe::print_error_and_crash("Ending Numbers of Message were not correct");
    }

    // Return struct!
    packet_message* retval = new packet_message;
    retval->data_id = std::string(message_id);
    retval->data_payload = payload;
    retval->data_size = (int) payload_size;
    return retval;
}

void cpp_to_python_win32pipe::threaded_data_import(shared_data* message_box){
    cpp_to_python_win32pipe::data_import_manager manager = cpp_to_python_win32pipe::data_import_manager(message_box);
}

// Data Exporting

cpp_to_python_win32pipe::data_export_manager::data_export_manager(){
}

cpp_to_python_win32pipe::data_export_manager::~data_export_manager(){
    CloseHandle(hPipe);
}

bool cpp_to_python_win32pipe::data_export_manager::is_connected(){
    return hPipe != INVALID_HANDLE_VALUE;
}

void cpp_to_python_win32pipe::data_export_manager::retry_connection(){
    if(is_connected()) return;
    hPipe = CreateFile(TEXT("\\\\.\\pipe\\cpp_to_python_pipe"), // Name
                   GENERIC_READ | GENERIC_WRITE,            // Read and Write
                   0,                                       // Does not share
                   NULL,                                    // Default Security
                   OPEN_EXISTING,                           // Opens an existing pipe
                   0,                                       // Default Attributes
                   NULL);
    std::cout << "H Value of: " << hPipe << std::endl;
}

void cpp_to_python_win32pipe::data_export_manager::export_data(std::string id, int size, unsigned char* data){
    cpp_to_python_win32pipe::print_log_message("Sending Message from C++ to Python.");
    byte_string* to_send = encode_message(id, size, data);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
                  to_send->data_str,   // Message
                  to_send->size,             
                  &dwWritten,       // Written Bytes
                  NULL);            // Not Overlapped
        std::string status_msg = "Sent " + std::to_string(to_send->size) + " bytes to Python.";
        cpp_to_python_win32pipe::print_log_message(status_msg);
        // Clean up byte Str
        delete[] to_send->data_str;
        delete to_send;
    }
    else {
        cpp_to_python_win32pipe::print_error_and_crash("Pipe on Python's end was closed, message could not be sent.");
    }
}


byte_string* cpp_to_python_win32pipe::data_export_manager::encode_message(std::string id, int size, unsigned char* data){
    // Calculate size
    int byte_str_size = (int) (magic_numbers_count * 2 + message_id_size + size);
    int passed_bytes = 0;

    // Create byte string
    unsigned char* byte_str = new unsigned char[byte_str_size];

    // Add starting bytes
    for(int i = 0; i < magic_numbers_count; i++){
        byte_str[i] = prefix_magic_numbers[i];
    }
    passed_bytes += magic_numbers_count;

    // Add data id (max of 16 error if over, print 0's if under)
    if(id.length() > 16) cpp_to_python_win32pipe::print_log_message("Id should be under 16 chrs, ending will be cut off!");
    const char* id_cstr = id.c_str();
    for(int i = 0; i < message_id_size; i++){
        if(id.length() > i){
            byte_str[passed_bytes + i] = (unsigned char) id_cstr[i];
        } else {
            byte_str[passed_bytes + i] = 0x00;
        }
    }
    passed_bytes += message_id_size;

    // Add payload data
    for(int i = 0; i < size; i++){
        byte_str[passed_bytes + i] = data[i];
    }
    passed_bytes += size;

    // Add ending nubmers
    for(int i = 0; i < magic_numbers_count; i++){
        byte_str[passed_bytes + i] = ending_magic_numbers[i];
    }

    // Create string object and return!
    byte_string* retval = new byte_string;
    retval->data_str = byte_str;
    retval->size = byte_str_size;
    return retval;
}