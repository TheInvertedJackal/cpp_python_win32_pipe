@echo off

if "%1" == "clean" (
    del *.o
    del main
) else (
    g++ main.cpp cpp\cpp_pipe_threads.cpp cpp\cpp_data_transfer_manager.cpp -o cpp_pipes
)