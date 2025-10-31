#ifndef CPP_THREAD_SAFE_QUEUE
#define CPP_THREAD_SAFE_QUEUE

#include <mutex>
#include <queue>

namespace cpp_to_python_win32pipe{
    template<typename T>
    class cpp_thead_safe_queue{
        private:
            int this_count;
            std::mutex queue_lock;
            std::queue<T> internal_queue;
        public:
            cpp_thead_safe_queue();
            ~cpp_thead_safe_queue();
            // Pops off 
            T pop();
            void push(T value);
            int count();
    };
}

#endif