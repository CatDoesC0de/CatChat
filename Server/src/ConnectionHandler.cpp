#include "ConnectionHandler.hpp"

#include "CatNet/BufferSnapshot.hpp"

#include <iostream>

ConnectionHandler::ConnectionHandler(CatNet::TCPSocket listeningSocket)
{
    m_pollList.push_back(pollfd{listeningSocket.descriptor(), POLLIN, 0});
}

void ConnectionHandler::poll()
{
    while (true)
    {
        if (::poll(m_pollList.data(), m_pollList.size(), 1) > 0)
        {
            if (m_pollList[0].revents & POLLIN)
            {
                CatNet::TCPSocket listeningSocket(m_pollList[0].fd);

                auto clientOptional = listeningSocket.accept();
                if (clientOptional.has_value())
                {
                    auto clientSocket = clientOptional.value();

                    pollfd pollInfo = pollfd{clientSocket.descriptor(), POLLIN, 0};

                    std::cout << "Accepted incoming connection.\n";

                    m_pollList.push_back(pollInfo);
                    m_connections.push_back(Connection(&m_pollList[m_pollList.size() - 1]));
                }
            }

            for (size_t i = 0; i < m_connections.size(); i++)
            {
                Connection &connection = m_connections[i];

                if (connection.pollInfo->revents & POLLIN)
                {

                    CatNet::Buffer &buffer = connection.buffer;
                    int bytesReceived = buffer.receive(connection.socket());

                    if (bytesReceived == -1)
                    {
                        std::cout << "Lost connection abruptly.\n";
                        continue;
                    }

                    if (bytesReceived == 0)
                    {
                        std::cout << "Connection close from client.\n";
                        
                        m_connections.erase(m_connections.begin() + i);
                        m_pollList.erase(m_pollList.begin() + i + 1);
                        continue;
                    }

                    switch (connection.processState)
                    {

                    case PACKET_SIZE:
                    {
                        if (buffer.size() == buffer.capacity())
                        {
                            CatNet::BufferSnapshot snapshot(buffer);
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

Connection::Connection(pollfd* _pollInfo)
    : pollInfo(_pollInfo), buffer(CatNet::Buffer(2)), processState(PACKET_SIZE)
{
}

CatNet::TCPSocket Connection::socket() const
{
    return CatNet::TCPSocket(pollInfo->fd);
}