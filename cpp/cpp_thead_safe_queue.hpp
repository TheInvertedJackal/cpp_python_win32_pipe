#ifndef CPP_THREAD_SAFE_QUEUE
#define CPP_THREAD_SAFE_QUEUE

#include <mutex>
#include <queue>
#include "transfer_data_struct.hpp"

namespace cpp_to_python_win32pipe{
    class cpp_thead_safe_queue{
        private:
            int this_count;
            std::mutex queue_lock;
            std::queue<packet_message*> internal_queue;
        public:
            // Inializes the queue
            cpp_thead_safe_queue();
            // Gets a queue value
            ~cpp_thead_safe_queue();
            // Pops off the first element, returns 0 if nothing is found
            packet_message* pop();
            // Pushes a value onto the back of the queue
            void push(packet_message* value);
            // Get the current count of
            int count();
    };
}

#endif