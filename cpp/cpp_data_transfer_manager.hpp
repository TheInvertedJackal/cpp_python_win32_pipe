#ifndef CPP_DATA_TRANSFER_MANAGER
#define CPP_DATA_TRANSFER_MANAGER

#include <string>
#include <map>
#include <mutex>
#include "transfer_data_struct.hpp"

namespace CppToPy_Win32 {

    typedef void (*payload_function)(transfer_data_struct*);

    struct thread_shared_data {
        bool other_program_connected;
        bool other_program_ready;
        bool payload_1_up;
        bool payload_2_up;
        uint32_t payload_1_size;
        uint32_t payload_2_size;
        unsigned char* payload_1;
        unsigned char* payload_2;
    };

    const std::string ready_str = "ready";
    const std::string handshake_str = "handshake";

    class cpp_data_transfer_manager
    {
        private:
            std::map<std::string, payload_function> callback_lookup_map;
        public:
            // If the connection has been established, if not trying to post a payload will cause it to error.
            bool is_connected();
            // Each callback has it's own lock, if this is called while a callback is in progress, it will hang
            payload_function get_callback(std::string id);
            // Adds a callback for a specific id, if a payload with an id is idenfied, the callback is called.
            void add_callback(std::string id, payload_function);
            // Post a payload with a given data id, size and byte payload.
            void post_payload(std::string data_id, uint32_t data_size, unsigned char* data_payload);
            // Used to signal that setup is done, and the data transfer can begin!
            void singal_ready();
            cpp_data_transfer_manager();
            ~cpp_data_transfer_manager();
    };

    // You'll need a ref back so that you can get access to any avalible data
    void thread_export(thread_shared_data* shared_data);
    // You need a ref to the parent to get the data callbacks.
    void thread_import(cpp_data_transfer_manager* parent);
    
}

#endif