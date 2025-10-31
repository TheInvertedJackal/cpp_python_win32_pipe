#ifndef CPP_TO_PY_MSG_SORTER
#define CPP_TO_PY_MSG_SORTER

#include "cpp_pipe_handler.hpp"
#include "cpp_thead_safe_queue.hpp"

namespace cpp_to_python_win32pipe{
    class cpp_msg_sorter{
        private:    
            // Called on a thread every second, used to 
            void process_messages();
        public:
            // Constructor, takes in a connection to sort
            cpp_msg_sorter(cpp_to_python* msg_receiver);
            // Destructor
            ~cpp_msg_sorter();

    };
} // namespace cpp_to_python_win32pipe


#endif