cmake_minimum_required(VERSION 3.1)
project(event_test CXX)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(event_test main.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(event_test PRIVATE Threads::Threads)


