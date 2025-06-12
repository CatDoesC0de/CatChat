#include <iostream>

#include "CatNet/TCPSocket.hpp"
#include "CatNet/Buffer.hpp"
#include "CatNet/packets/serverbound/ServerboundLoginPacket.hpp"

int main()
{
    auto socket = std::move(CatNet::TCPSocket::Create().value());

    if (!socket.connect("127.0.0.1", 7000))
    {
        std::cerr << "Failed to connect to remote host.\n";
        return -1;
    }

    std::cout << "Connected to remote host...\n";
    
    CatNet::ServerboundLoginPacket packet = {};
    std::cin >> packet.requestedUsername;

    CatNet::Buffer buffer(1024);
    encode_packet_with_header(packet, buffer);
    buffer.send(socket, buffer.readable_bytes());
}
