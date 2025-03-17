cmake_minimum_required(VERSION 3.10)
project(VK_Intership)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(${CMAKE_SOURCE_DIR}/input.txt ${CMAKE_BINARY_DIR}/input.txt COPYONLY)

add_executable(VK_Intership main.cpp graph.cpp)