cmake_minimum_required(VERSION 3.5)

project(EchoServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp server.cpp clientnode.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ev)
