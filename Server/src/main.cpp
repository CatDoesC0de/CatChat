#include <iostream>

#include "CatNet/TCPSocket.hpp"
#include "CatNet/Buffer.hpp"
#include "CatNet/BufferSnapshot.hpp"

#include "ConnectionHandler.hpp"

#include <vector>
#include <poll.h>

#define PORT 7000

int main()
{
    auto socketOptional = CatNet::TCPSocket::Create();
    
    if (!socketOptional.has_value())
    {
        std::cerr << "Failed to create server socket!\n";
        return -1;
    }

    auto listeningSocket = socketOptional.value();

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

    ConnectionHandler connectionHandler(listeningSocket);
    connectionHandler.poll();
}