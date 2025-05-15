#include <iostream>

#include "CatNet/TCPSocket.hpp"

int main()
{
    auto socket = std::move(CatNet::TCPSocket::Create().value());

    if (!socket.connect("127.0.0.1", 7000))
    {
        std::cerr << "Failed to connect to remote host.\n";
        return -1;
    }

    std::cout << "Connected to remote host...\n";

    CatNet::Buffer buffer(8000 + 2);
    buffer.int16(8000);
    for (int i = 0; i < (8000 / 4); i++)
    {
        buffer.int32(i);
    }

    int bytesSent = socket.send(buffer);
    std::cout << "Bytes Sent: " << bytesSent << '\n';
}
