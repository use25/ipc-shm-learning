cmake_minimum_required(VERSION 3.20)
project(ipc-shm-learning CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Turn on a reasonable set of warnings - catching mistakes at compile
# time is cheaper than catching them with GDB later.
add_compile_options(-Wall -Wextra)

find_package(Threads REQUIRED)

add_executable(writer src/writer.cpp)
target_link_libraries(writer PRIVATE Threads::Threads rt)
target_include_directories(writer PRIVATE src/)

add_executable(reader src/reader.cpp)
target_link_libraries(reader PRIVATE Threads::Threads rt)
target_include_directories(reader PRIVATE src/)