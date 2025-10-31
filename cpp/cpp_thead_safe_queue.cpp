#include <cpp_thead_safe_queue.hpp>

using namespace cpp_to_python_win32pipe;
using namespace std;

template <typename T>
cpp_thead_safe_queue<T>::cpp_thead_safe_queue() :
    ,count{0}{

}

template <typename T>
T cpp_thead_safe_queue<T>::pop(){
    queue_lock.lock();
    if(count == 0){
        queue_lock.unlock();
        return nullptr;
    }
    T retval = internal_queue.front();
    internal_queue.pop();
    this_count--;
    queue_lock.unlock();
    return retval;
}

template <typename T>
void cpp_thead_safe_queue<T>::push(T value){
    queue_lock.lock();
    this_count++;
    internal_queue.push(value);
    queue_lock.unlock();
}

template <typename T>
int cpp_thead_safe_queue<T>::count(){
    queue_lock.lock();
    int ret_val = count;
    queue_lock.unlock();
    return retval;
}