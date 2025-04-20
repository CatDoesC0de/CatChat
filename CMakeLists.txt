cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_BUILD_TYPE Debug)

project(CatChat)

add_subdirectory(CatNet)
add_subdirectory(Server)
add_subdirectory(Client)