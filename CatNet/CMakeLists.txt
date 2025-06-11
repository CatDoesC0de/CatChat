project(CatNet)

add_library(${PROJECT_NAME} STATIC 
    src/TCPSocket.cpp 
    src/BufferView.cpp 
    src/Buffer.cpp

    src/packets/serverbound/ServerboundLoginPacket.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/CatNet) 
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include) 
