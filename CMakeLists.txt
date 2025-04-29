cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_BUILD_TYPE Debug)

project(CatChat)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_subdirectory(CatNet)
add_subdirectory(Server)
add_subdirectory(Client)
