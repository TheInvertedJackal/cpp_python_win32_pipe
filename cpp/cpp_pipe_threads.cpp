#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>

#include "cpp_data_transfer_manager.hpp"

using namespace CppToPy_Win32;

// Export Process
// Setup pipe to python
// Begin loop
// Check 1
// Check 2
// Repeat
// If 1 (or 2)
// Pull and send data
// Flip bool
// Base From: https://stackoverflow.com/questions/26561604/create-windows-named-pipe-in-c
void thread_export(thread_shared_data* shared_data){
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;


    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\cpp_to_python_pipe"),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                            1,
                            1024 * 16,
                            1024 * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                /* add terminating zero */
                buffer[dwRead] = '\0';

                /* do something with data in buffer */
                printf("%s", buffer);
            }
        }

        DisconnectNamedPipe(hPipe);
    }
    return;
}


// Import Process
// Loop
// Hang on Pipe
// Get callback
// Does callback exist?
// Base from: https://stackoverflow.com/questions/26561604/create-windows-named-pipe-in-c
void thread_import(cpp_data_transfer_manager* parent){
    HANDLE hPipe;
    DWORD dwWritten;


    hPipe = CreateFile(TEXT("\\\\.\\pipe\\python_to_cpp_pipe"), 
                       GENERIC_READ | GENERIC_WRITE, 
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
                  "Hello Pipe\n",
                  12,   // = length of string + terminating '\0' !!!
                  &dwWritten,
                  NULL);

        CloseHandle(hPipe);
    }

    return;
}

// Useful Refs:
// - https://learn.microsoft.com/en-us/windows/win32/ipc/multithreaded-pipe-server
// - https://learn.microsoft.com/en-us/windows/win32/ipc/named-pipe-client
// - https://stackoverflow.com/questions/26561604/create-windows-named-pipe-in-c