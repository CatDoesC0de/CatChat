#include <iostream>

#include "CatNet/TCPSocket.hpp"
#include "CatNet/Buffer.hpp"

#include "network_loop.hpp"

#define PORT 7000

int main()
{
    auto socketOptional = CatNet::TCPSocket::Create();
    
    if (!socketOptional.has_value())
    {
        std::cerr << "Failed to create server socket!\n";
        return -1;
    }

    auto listeningSocket = std::move(socketOptional.value());

    if (!listeningSocket.bind("127.0.0.1", PORT))
    {
        std::cerr << "Failed to bind on port " << PORT << ". Is it already in use?\n";
        return -1;
    }

    if (!listeningSocket.listen(3))
    {
        std::cerr << "Failed to bind on port " << PORT << ". Is it already in use?\n";
        return -1;
    }

    std::cout << "Server listening on port " << PORT << '\n';

    listeningSocket.blocking(false);

    poll(listeningSocket);
}
