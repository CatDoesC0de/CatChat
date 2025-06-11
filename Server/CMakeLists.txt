project(Server)

add_executable(${PROJECT_NAME} src/main.cpp src/Connection.cpp src/CatChatServer.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE CatNet)
