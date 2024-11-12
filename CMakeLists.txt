cmake_minimum_required(VERSION 3.16)
project(Multi_Threaded_Memory_Allocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(Multi_Threaded_Memory_Allocator
        tests/MemoryPool.cpp
        tests/MemoryPool.h
        tests/memoryPoolSimpleTests.cpp
        tests/memoryPoolSimpleTests.h
        tests/multiThreadingTests.cpp
        tests/multiThreadingTests.h
        main.cpp)

target_link_libraries(Multi_Threaded_Memory_Allocator pthread)
