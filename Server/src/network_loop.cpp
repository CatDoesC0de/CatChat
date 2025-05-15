#include "network_loop.hpp"

#include "CatNet/BufferView.hpp"

#include <vector>
#include <iostream>

inline static std::vector<pollfd> g_pollList;
inline static std::vector<Connection> g_connections;

Connection::Connection(pollfd* _pollInfo, CatNet::TCPSocket _socket)
    : pollInfo(_pollInfo), socket(std::move(_socket)), buffer(CatNet::Buffer(2)), processState(PACKET_SIZE)
{}

void poll(const CatNet::TCPSocket& listeningSocket)
{
    g_pollList.push_back(pollfd{listeningSocket.descriptor(), POLLIN, 0});

    while (true)
    {
        if (::poll(g_pollList.data(), g_pollList.size(), 1) > 0)
        {
            if (g_pollList[0].revents & POLLIN)
            {

                auto clientOptional = listeningSocket.accept();

                if (clientOptional.has_value())
                {
                    CatNet::TCPSocket& clientSocket = clientOptional.value();

                    pollfd pollInfo = pollfd{clientSocket.descriptor(), POLLIN, 0};

                    std::cout << "Accepted incoming connection.\n";

                    g_pollList.push_back(pollInfo);
                    g_connections.push_back(Connection(&g_pollList[g_pollList.size() - 1], std::move(clientSocket)));
                }
            }

            for (size_t i = 0; i < g_connections.size(); i++)
            {
                Connection &connection = g_connections[i];

                if (connection.pollInfo->revents & POLLIN)
                {

                    CatNet::Buffer &buffer = connection.buffer;
                    int bytesReceived = buffer.receive(connection.socket);

                    if (bytesReceived == -1)
                    {
                        std::cout << "Lost connection abruptly.\n";
                        continue;
                    }

                    if (bytesReceived == 0)
                    {
                        std::cout << "Connection close from client.\n";
                        
                        g_connections.erase(g_connections.begin() + i);
                        g_pollList.erase(g_pollList.begin() + i + 1);
                        continue;
                    }

                    switch (connection.processState)
                    {

                    case PACKET_SIZE:
                    {
                        if (buffer.size() == buffer.capacity())
                        {
                            CatNet::BufferView snapshot(buffer);
                            uint16_t packetSize = snapshot.int16();

                            std::cout << "Packet Size: " << packetSize << " bytes\n";
                            buffer.reserve(packetSize);
                            buffer.clear(); //Reset write pointer to 0 so we can read full packet.

                            connection.processState = PACKET_PAYLOAD;
                        }
                        break;
                    }

                    case PACKET_PAYLOAD:
                    {
                        if (buffer.size() == buffer.capacity())
                        {
                            // Handle the fully received packet
                            std::cout << "Read full packet (" << buffer.size() << ") bytes.\n";
                            connection.processState = PACKET_SIZE;
                        }
                        break;
                    }

                    }
                }
            }
        }
    }
}
