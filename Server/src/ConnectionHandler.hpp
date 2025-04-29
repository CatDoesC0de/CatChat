#pragma once

#include "CatNet/TCPSocket.hpp"

#include <vector>
#include <poll.h>

enum PacketProcessState
{
    PACKET_SIZE, PACKET_PAYLOAD
};

struct Connection
{
    Connection(pollfd* _pollInfo, CatNet::TCPSocket& socket);

    pollfd* pollInfo;
    CatNet::TCPSocket socket;
    CatNet::Buffer buffer;
    PacketProcessState processState;
};

class ConnectionHandler
{

    public:

    ConnectionHandler(CatNet::TCPSocket listeningSocket);

    void poll();

    private:

    std::vector<pollfd> m_pollList;
    std::vector<Connection> m_connections;

};
