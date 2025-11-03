#include "cpp_thead_safe_queue.hpp"

using namespace cpp_to_python_win32pipe;
using namespace std;

cpp_thead_safe_queue::cpp_thead_safe_queue() :
    this_count{0}{

}

packet_message* cpp_thead_safe_queue::pop(){
    queue_lock.lock();
    if(this_count == 0){
        queue_lock.unlock();
        return nullptr;
    }
    packet_message* retval = internal_queue.front();
    internal_queue.pop();
    this_count--;
    queue_lock.unlock();
    return retval;
}

void cpp_thead_safe_queue::push(packet_message* value){
    queue_lock.lock();
    this_count++;
    internal_queue.push(value);
    queue_lock.unlock();
}

int cpp_thead_safe_queue::count(){
    queue_lock.lock();
    int retval = this_count;
    queue_lock.unlock();
    return retval;
}