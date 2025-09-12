#include "cpp_data_transfer_manager.hpp"

using namespace std;
using namespace CppToPy_Win32;

cpp_data_transfer_manager::cpp_data_transfer_manager(){

}

cpp_data_transfer_manager::~cpp_data_transfer_manager(){

}

// NO LOOP!
// Management Process
// Setup empty map
// Created shared memeory struct
// Spawn the Export Thread (Detached)
// Spawn the Improt Thread (Detached)

// signal_ready()
// singal the other program that you're set up

// post_payload()
// place paylod info in shared data
// flip boolean

// add_callback()
// Add callback to map

// get_callback()
// Return a callback, if not in the map, return null pointer

// Export Process
// Setup thread
// Begin loop
// Check 1
// Check 2
// Repeat
// If 1 (or 2)
// Pull and send data
// Flip bool


// Import Process
// Loop
// Hang on Pipe
// Get callback
// Does callback exist?