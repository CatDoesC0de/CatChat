project(Server)

add_executable(${PROJECT_NAME} src/main.cpp src/network_loop.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CatNet)
