project(Server)

add_executable(${PROJECT_NAME} src/main.cpp src/ConnectionHandler.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CatNet)