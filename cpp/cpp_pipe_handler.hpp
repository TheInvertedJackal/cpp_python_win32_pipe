#ifndef CPP_PIPE_HANDLER
#define CPP_PIPE_HANDLER

#include "transfer_data_struct.hpp"
#include "cpp_pipe_methods.hpp"
#include <vector>

using namespace cpp_to_python_win32pipe;

namespace cpp_to_python_win32pipe{
    
    class cpp_to_python{
        private:
            shared_data* message_box = nullptr;
            data_export_manager exporter;
            std::vector<packet_message*>* pull_msgs();
        public:
            // Sets up pipes, threads and tries intial connecton
            cpp_to_python();
            // Sends a message to python
            void send_msg(std::string id, int message_size, unsigned char* byte_message);
            // Returns 0 if no new messages, otherwise returns messages from python
            std::vector<packet_message*>* check_msgs();
            // Retrys connecting to the Python program.
            void retry_connection();
            // Returns true if there's been a connection to python
            bool is_connected();
    };
};

#endif