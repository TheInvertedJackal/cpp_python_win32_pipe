# ifndef CPP_TO_PYTHON_PIPE_METHODS
# define CPP_TO_PYTHON_PIPE_METHODS

#include <windows.h> 
#include <stdio.h>
#include <tchar.h>

#include "transfer_data_struct.hpp"

namespace cpp_to_python_win32pipe {
    
    const int BUFFER_SIZE = 1024;
    const int magic_numbers_count = 4;
    const int message_id_size = 16; // 16 as
    const int max_msg_placement_tries = 32;
    const unsigned char prefix_magic_numbers[] = { 0x32, 0xf5, 0xe4, 0x97 };
    const unsigned char ending_magic_numbers[] = { 0x89, 0xe4, 0xaa, 0xb3 };
    
    class data_export_manager {
        private:
            HANDLE hPipe = INVALID_HANDLE_VALUE;
            DWORD dwWritten = 0;
            // Encodes the message to a byte string value
            byte_string* encode_message(std::string id, int size, unsigned char* data);
        public:
            // This does NOT free *data. You'll need to do that yourself!
            void export_data(std::string id, int size, unsigned char* data);
            // If the pipe is connected to python
            bool is_connected();
            // Attempt to reconnect to python server
            void retry_connection();
            // THIS DOES NOT AUTO CONNECT ON OPEN! You'll need to do that yourself.
            data_export_manager();
            ~data_export_manager();
    };

    // Class is created and managed within thread
    class data_import_manager {
        private:
            int next_message_index = shared_msg_buffer_size - 1;
            shared_data* message_box = nullptr;
            HANDLE hPipe = 0;
            unsigned char buffer[BUFFER_SIZE];
            DWORD dwRead = 0;

            // Decodes the read byte string
            packet_message* decode_message(int size, unsigned char* data);
            void check_for_client();
            void read_messages();
            void place_message(packet_message *);
        public:
            data_import_manager(shared_data* message_box);
    };
    
    // Pull Message (Async with shared memeory)
    void threaded_data_import(shared_data* message_box);

    void print_log_message(std::string message);

    void print_error(std::string message);

    void print_error_and_crash(std::string message);
}

# endif