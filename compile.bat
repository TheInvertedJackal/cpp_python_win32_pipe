@echo off

if "%1" == "server" (
    @echo on
    g++ server_test.cpp cpp\cpp_pipe_methods.cpp -o cpp_server
    @echo off
    goto exit
)

if "%1" == "client" (
    @echo on
    g++ client_test.cpp cpp\cpp_pipe_methods.cpp -o cpp_client
    @echo off
    goto exit
)

if "%1" == "dual" (
    @echo on
    g++ cpp_dual_test.cpp cpp\cpp_pipe_methods.cpp cpp\cpp_pipe_handler.cpp cpp\cpp_thead_safe_queue.cpp cpp\cpp_msg_handler_wrapper.cpp -o cpp_dual
    @echo off
    goto exit
)


if "%1" == "test" (
    @echo on
    g++ cpp_load_test.cpp cpp\cpp_pipe_methods.cpp cpp\cpp_pipe_handler.cpp -o cpp_load
    @echo off
    goto exit
)


echo "The option %1 is not reconized!"

:exit