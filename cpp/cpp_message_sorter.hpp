#ifndef CPP_TO_PY_MSG_SORTER
#define CPP_TO_PY_MSG_SORTER

#include "cpp_pipe_handler.hpp"
#include "cpp_thead_safe_queue.hpp"
#include <vector>
#include <map>


namespace cpp_to_python_win32pipe{
    class cpp_msg_sorter{
        private:    
            // The maximum numbers of times that can be pulled from a queue at a time.
            const int MAX_CHECK_COUNT = 10;
            bool processing_messages;
            cpp_to_python_win32pipe::cpp_to_python* msg_receiver;
            // Called on a thread every second, used to sort messages into proper queues
            void process_messages();
            std::map<std::string, cpp_to_python_win32pipe::cpp_thead_safe_queue*> listeners;
        public:
            // Constructor, takes in a connection to sort
            cpp_msg_sorter(cpp_to_python_win32pipe::cpp_to_python* msg_rec);
            // Destructor
            ~cpp_msg_sorter();
            // Adds a listener for a given value id
            void add_listener(std::string id);
            // Trys to get a vector array of messages, returns nullptr if none are found.
            std::vector<packet_message*>* get_message_for_id(std::string id);
    };
} // namespace cpp_to_python_win32pipe


#endif