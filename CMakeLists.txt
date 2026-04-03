cmake_minimum_required(VERSION 3.10)
project(OrderFlow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Winsock2 library
if(WIN32)
    link_libraries(ws2_32)
endif()

# Client executable
file(GLOB_RECURSE CLIENT_SOURCES "client/src/*.cpp")
add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/client/include")

# Server executable
file(GLOB_RECURSE SERVER_SOURCES "server/src/*.cpp")
add_executable(server ${SERVER_SOURCES})
target_include_directories(server PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/server/include")
