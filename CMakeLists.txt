cmake_minimum_required(VERSION 3.10)
project(order-book)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

#enable_testing()

add_executable(order-book src/main.cpp)


#TEST:
#add_test(NAME test_name COMMAND executable)
